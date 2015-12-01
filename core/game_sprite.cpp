#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <GL/glfw.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "../common/shader.hpp"
#include "../common/texture.hpp"
#include "../common/objloader.hpp"
#include "../common/lodepng.hpp"

#include <SOIL/SOIL.h>
#include <SOIL/image_helper.h>
#include <SOIL/image_DXT.h>
#include <SOIL/stb_image_aug.h>
//#include <SOIL/stbi_DDS_aug.h>
//#include <SOIL/stbi_DDS_aug_c.h>

#include "game_sprite.hpp"
#include "gl_err_log.hpp"

#include <btBulletDynamicsCommon.h>

// Constructor/Destructor {{{
GameSprite::GameSprite(int numTriangles_arg, std::string modelFile_arg, std::string textureFile_arg)
{
        texture = new GLuint;
        vertexBuffer = new GLuint;
        uvBuffer = new GLuint;

        numTriangles = numTriangles_arg;
        modelFile = modelFile_arg;
        textureFile = textureFile_arg;
}

GameSprite::~GameSprite()
{
        //fprintf( stderr, "Quit requested, cleaning up...\n" );

        glDeleteBuffers(1 , vertexBuffer);
        glDeleteBuffers(1 , uvBuffer);

        delete texture;
}
// }}}

GameSprite* GameSprite::init(void)
{
        //fprintf( stderr, "Loading object from disk\n" );

        loadTexture();
        loadModel();

        return 0;
}


GameSprite* GameSprite::loadTexture()
{
        //fprintf( stderr, "Loading object TEXTURE\n" );

        //*texture = loadBMP_custom(textureFile.c_str());
        *texture = loadTGA_glfw(textureFile.c_str());
        //*texture = SOIL_load_OGL_texture(
        //        textureFile.c_str(),
        //        SOIL_LOAD_AUTO,
        //        SOIL_CREATE_NEW_ID,
        //        SOIL_FLAG_INVERT_Y
        //);

        //*texture = SOIL_load_OGL_texture(
        //        textureFile.c_str(),
        //        SOIL_LOAD_AUTO,
        //        //SOIL_CREATE_NEW_ID,
        //        *texture,
        //        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
        //);

        return this;
}

GameSprite* GameSprite::loadModel()
{
        //fprintf( stderr, "Loading object MESH\n" );

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals; // Won't be used at the moment.
        bool res = loadOBJ(modelFile.c_str(), vertices, uvs, normals);

        // Load it into a VBO
        glGenBuffers(1, vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, *vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

        // Texture (UV) Buffer
        glGenBuffers(1, uvBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, *uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

        return 0;
}

GameSprite* GameSprite::setNumTriangles(int numTriangles_arg)
{
        numTriangles = numTriangles_arg;

        return this;
}

int GameSprite::getNumTriangles()
{
        return numTriangles;
}

GLuint* GameSprite::getTexture()
{
        return texture;
}

GLuint* GameSprite::getVertexBuffer()
{
        return vertexBuffer;
}

GLuint* GameSprite::getUvBuffer()
{
        return uvBuffer;
}
