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

#include "game_camera.hpp"
#include "gl_err_log.hpp"

#include <btBulletDynamicsCommon.h>

// Constructor/Destructor {{{
GameCamera::GameCamera()
{
}

GameCamera::~GameCamera()
{
}
// Constructor/Destructor }}}

GameCamera* GameCamera::init(void)
{
        FOV             = 60.f;
        aspectRatio     = 16.0f / 9.0f;
        minRange        = 0.1f;
        maxRange        = 1000.0f;
        horizontalAngle = 3.14f;
        verticalAngle   = -0.449670f;

        x = 0.f;
        y = 0.f;
        z = 0.f;

        initProjection();

        return this;
}

GameCamera* GameCamera::init(float FOV_arg, float aspectRatio_arg, float minRange_arg, float maxRange_arg)
{
        FOV = FOV_arg;
        aspectRatio = aspectRatio_arg;
        minRange = minRange_arg;
        maxRange = maxRange_arg;

        initProjection();

        return this;
}

GameCamera* GameCamera::setFOV(float FOV_arg)
{
        FOV = FOV_arg;

        return this;
}

GameCamera* GameCamera::setAspectRatio(float aspectRatio_arg)
{
        aspectRatio = aspectRatio_arg;

        return this;
}

GameCamera* GameCamera::setMinRange(float minRange_arg)
{
        minRange = minRange_arg;

        return this;
}

GameCamera* GameCamera::setMaxRange(float maxRange_arg)
{
        maxRange = maxRange_arg;

        return this;
}

float GameCamera::getFOV(void)
{
        FOV;
}

float GameCamera::getAspectRatio(void)
{
        aspectRatio;
}

float GameCamera::getMaxRange(void)
{
        maxRange;
}

float GameCamera::getMinRange(void)
{
        minRange;
}

GameCamera* GameCamera::setPerspectiveProjection(void)
{
        projection = perspective(
                        FOV,
                        aspectRatio,
                        minRange,
                        maxRange
                        );

        return this;
}

GameCamera* GameCamera::setOrthoProjection(void)
{
        projection = ortho(
                        -10.0f,
                        10.0f,
                        -10.0f,
                        10.0f,
                        0.0f,
                        100.0f
                        ); // In world coordinates

        return this;
}

GameCamera* GameCamera::initProjection(void)
{
        setPerspectiveProjection();

        return this;
}

GameCamera* GameCamera::adjustVerticalAngle(float increase)
{
        verticalAngle += increase;

        fprintf(stderr, "new vertical angle: %f\n", verticalAngle);

        return this;
}

GameCamera* GameCamera::adjustHorizontalAngle(float increase)
{
        horizontalAngle += increase;

        fprintf(stderr, "new horizontal angle: %f\n", horizontalAngle);

        return this;
}

float GameCamera::getHorizontalAngle()
{
        return horizontalAngle;
}

float GameCamera::getVerticalAngle()
{
        return verticalAngle;
}

GameCamera* GameCamera::setVerticalAngle(float angle)
{
        verticalAngle = angle;

        fprintf(stderr, "new vertical angle: %f\n", verticalAngle);

        return this;
}

GameCamera* GameCamera::setHorizontalAngle(float angle)
{
        horizontalAngle = angle;

        fprintf(stderr, "new horizontal angle: %f\n", horizontalAngle);

        return this;
}

mat4 GameCamera::getView()
{
        vec3 direction(
                        cos(verticalAngle) * sin(horizontalAngle),
                        sin(verticalAngle),
                        cos(verticalAngle) * cos(horizontalAngle)
                      );

        vec3 right(
                        sin(horizontalAngle - 3.14f/2.0f),
                        0,
                        cos(horizontalAngle - 3.14f/2.0f)
                  );

        vec3 up = cross(right, direction);

        vec3 position = getPosition();

        return lookAt(
                        position,
                        position+direction,
                        up
                     );
}

GameCamera* GameCamera::setLookAt(mat4 look_at_arg)
{
        view = look_at_arg;

        return this;
}

GameCamera* GameCamera::setLookAt(vec3 position, vec3 direction, vec3 up)
{
        view = lookAt(
                        position,
                        position+direction,
                        up
                     );

        return this;
}

vec3 GameCamera::getPosition(void)
{
        return vec3(x, y, z);
}

mat4 GameCamera::getProjection(void)
{
        return projection;
}

GameCamera* GameCamera::setCoordX(float x_arg)
{
        x = x_arg;

        return this;
}

GameCamera* GameCamera::setCoordY(float y_arg)
{
        y = y_arg;

        return this;
}

GameCamera* GameCamera::setCoordZ(float z_arg)
{
        z = z_arg;

        return this;
}

GameCamera* GameCamera::adjustCoordX(float x_arg)
{
        x += x_arg;

        return this;
}

GameCamera* GameCamera::adjustCoordY(float y_arg)
{
        y += y_arg;

        return this;
}

GameCamera* GameCamera::adjustCoordZ(float z_arg)
{
        z += z_arg;

        return this;
}

float GameCamera::getCoordX(void)
{
        return x;
}

float GameCamera::getCoordY(void)
{
        return y;
}

float GameCamera::getCoordZ(void)
{
        return z;
}
// }}}
