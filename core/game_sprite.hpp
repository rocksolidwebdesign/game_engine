#ifndef GAME_SPRITE_H
#define GAME_SPRITE_H

#include <string>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SOIL/SOIL.h>
#include <SOIL/image_helper.h>
#include <SOIL/image_DXT.h>
#include <SOIL/stb_image_aug.h>

#include <btBulletDynamicsCommon.h>

class GameSprite
{
        std::string modelFile;
        std::string textureFile;

        GLuint *vertexBuffer;
        GLuint *uvBuffer;
        GLuint *texture;

        int numTriangles;

        GameSprite* loadModel();
        GameSprite* loadTexture();

public:
        GameSprite();
        GameSprite(int numTriangles_arg, std::string modelFile_arg, std::string textureFile_arg);
        ~GameSprite();

        GameSprite* init();

        int getNumTriangles();
        GLuint* getTexture();
        GLuint* getVertexBuffer();
        GLuint* getUvBuffer();

        GameSprite* setNumTriangles(int x_arg);
};

#endif
