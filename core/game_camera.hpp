#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using glm::mat4;
using glm::vec3;

#include "game_object.hpp"

class GameCamera
{
        float x;
        float y;
        float z;

        float FOV;
        float aspectRatio;
        float minRange;
        float maxRange;
        float horizontalAngle;
        float verticalAngle;

        mat4 projection, view;

        GameCamera* initView(void);
        GameCamera* initProjection(void);
        GameCamera* setPerspectiveProjection(void);
        GameCamera* setOrthoProjection(void);

public:

        GameCamera();
        ~GameCamera();

        GameCamera* init(void);
        GameCamera* init(float, float, float, float);

        mat4 getProjection(void);
        mat4 getView(void);

        GameCamera* setLookAt(mat4);
        GameCamera* setLookAt(vec3, vec3, vec3);
        GameCamera* setFOV(float);
        GameCamera* setAspectRatio(float);
        GameCamera* setMinRange(float);
        GameCamera* setMaxRange(float);
        GameCamera* adjustVerticalAngle(float);
        GameCamera* adjustHorizontalAngle(float);
        GameCamera* setVerticalAngle(float);
        GameCamera* setHorizontalAngle(float);
        GameCamera* readjustView(void);

        float getFOV(void);
        float getAspectRatio(void);
        float getMaxRange(void);
        float getMinRange(void);

        GameCamera* setCoordX(float);
        GameCamera* setCoordY(float);
        GameCamera* setCoordZ(float);
        GameCamera* adjustCoordX(float);
        GameCamera* adjustCoordY(float);
        GameCamera* adjustCoordZ(float);

        vec3 getPosition(void);
        float getCoordX(void);
        float getCoordY(void);
        float getCoordZ(void);
        float getVerticalAngle();
        float getHorizontalAngle();
};

#endif
