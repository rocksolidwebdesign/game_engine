#ifndef GAME_SHADER_H
#define GAME_SHADER_H

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "game_object.hpp"
#include "game_camera.hpp"

class GameShader
{
        GLuint *mVertexArrayID,
               *mProgramID,
               *mMatrixID,
               *mTextureID;

        GameCamera *camera;

        GameShader* render(GameObject *model);
        GameShader* bind(GameObject *model);
        GameShader* bindMatrix(GameObject *model);
        GameShader* bindTexture(GameObject *model);
        GameShader* bindVertices(GameObject *model);

public:

        GameShader();
        GameShader(GameCamera*);
        ~GameShader();

        GameShader* init(GameCamera*);

        GameShader* draw(GameObject*);

        GameShader* newFrame();
        GameShader* reset();

        GLuint* getProgram();
        GLuint* getMatrix();
        GLuint* getTexture();
};

#endif
