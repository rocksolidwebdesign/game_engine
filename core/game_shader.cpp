#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>

#include <GL/glfw.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "../common/shader.hpp"
#include "../common/texture.hpp"
#include "../common/objloader.hpp"

#include "game_sprite.hpp"
#include "game_object.hpp"
#include "game_camera.hpp"
#include "game_shader.hpp"
#include "gl_err_log.hpp"

#include <btBulletDynamicsCommon.h>

GameShader::GameShader()
{
        mProgramID = new GLuint;
        mVertexArrayID = new GLuint;
        mMatrixID = new GLuint;
        mTextureID = new GLuint;
}

GameShader::~GameShader()
{
        GLERR(glDeleteProgram(*mProgramID));
        GLERR(glDeleteTextures(1 , mMatrixID));
        GLERR(glDeleteTextures(1 , mTextureID));
        GLERR(glDeleteVertexArrays(1 , mVertexArrayID));
}

GameShader* GameShader::newFrame()
{
        //fprintf( stderr, "NEW FRAME\n" );

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

        GLERR(glUseProgram(*mProgramID)); // Use our shader

        return this;
}

GameShader* GameShader::reset()
{
        //fprintf( stderr, "RESET\n" );

        GLERR(glfwSwapBuffers()); // Swap buffers

        return this;
}

GameShader* GameShader::init(GameCamera *camera_arg)
{
        camera = camera_arg;

        // might as well bind the vertex arrays here as well
        // since it doesn't fit nicely anywhere else
        //fprintf( stderr, "Bind vertex arrays\n" );

        GLERR(glGenVertexArrays(1, mVertexArrayID));
        GLERR(glBindVertexArray(*mVertexArrayID));

        //fprintf( stderr, "Loading Shaders\n" );

        // Create the shaders
        // Create and compile our GLSL program from the shaders
        *mProgramID = LoadShaders(
                        "shaders/ModelViewProjection.vertexshader",
                        "shaders/UvMappedTexture.fragmentshader"
                        );

        GLERR(*mTextureID = glGetUniformLocation(*mProgramID, "myTextureSampler"));
        GLERR(*mMatrixID  = glGetUniformLocation(*mProgramID, "MVP")); // Get a handle for our "MVP" uniform

        return this;
}

GameShader* GameShader::draw(GameObject *model)
{
        //fprintf( stderr, "Draw model\n" );

        bind(model);
        render(model);

        return this;
}

GameShader* GameShader::bind(GameObject *model)
{
        bindMatrix(model);
        bindTexture(model);
        bindVertices(model);

        return this;
}

GameShader* GameShader::bindMatrix(GameObject *model)
{
        //fprintf( stderr, "bind matrix\n" );

        // get matrix transformation MVP to redraw all
        // vertices relative to the camera angle and new object positions
        glm::mat4 Position   = model->getPosition();
        glm::mat4 Rotation   = model->getRotation();
        glm::mat4 Scaling    = model->getScaling();
        glm::mat4 View       = camera->getView();
        glm::mat4 Projection = camera->getProjection();

        glm::mat4 MVP        = Projection * View * Position * Rotation * Scaling;

        // Apply the transformation
        glUniformMatrix4fv(*mMatrixID, 1, GL_FALSE, &MVP[0][0]);
}

GameShader* GameShader::bindTexture(GameObject *model)
{
        //fprintf( stderr, "activate texture\n" );
        GLERR(glActiveTexture(GL_TEXTURE0)); // Bind our texture in Texture Unit 0

        //fprintf( stderr, "bind texture\n" );
        GLERR(glBindTexture(GL_TEXTURE_2D, *(model->getTexture())));

        //fprintf( stderr, "set uniform\n" );
        GLERR(glUniform1i(*mTextureID, 0)); // Set our "myTextureSampler" sampler to user Texture Unit 0

        //fprintf( stderr, "complete\n" );
}

GameShader* GameShader::bindVertices(GameObject *model)
{
        //fprintf( stderr, "Bind vertices\n" );

        // 1rst attribute buffer : vertices
        GLERR(glEnableVertexAttribArray(0));
        GLERR(glBindBuffer(GL_ARRAY_BUFFER, *(model->getVertexBuffer())));
        GLERR(glVertexAttribPointer(
                                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                                3,                  // size
                                GL_FLOAT,           // type
                                GL_FALSE,           // normalized?
                                0,                  // stride
                                (void*)0            // array buffer offset
                                ));

        // 2nd attribute buffer : UVs
        GLERR(glEnableVertexAttribArray(1));
        GLERR(glBindBuffer(GL_ARRAY_BUFFER, *(model->getUvBuffer())));
        GLERR(glVertexAttribPointer(
                                1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                                2,                                // size : U+V => 2
                                GL_FLOAT,                         // type
                                GL_FALSE,                         // normalized?
                                0,                                // stride
                                (void*)0                          // array buffer offset
                                ));

        return this;
}

GameShader* GameShader::render(GameObject *model)
{
        //fprintf( stderr, "Really draw really!\n" );

        int numTriangles = model->getNumTriangles();

        //fprintf( stderr, "num triangles: %d\n", numTriangles );

        // Draw !
        GLERR(glDrawArrays(GL_TRIANGLES, 0, numTriangles*3)); // 3 indices starting at 0 -> 1 triangle

        GLERR(glDisableVertexAttribArray(0));
        GLERR(glDisableVertexAttribArray(1));

        return this;
}

GLuint* GameShader::getProgram(void)
{
        return mProgramID;
}

GLuint* GameShader::getMatrix(void)
{
        return mMatrixID;
}

GLuint* GameShader::getTexture(void)
{
        return mTextureID;
}
