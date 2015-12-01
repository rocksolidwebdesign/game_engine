#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>

#include <GL/glfw.h>

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include "../common/shader.hpp"
#include "../common/texture.hpp"
#include "../common/objloader.hpp"

#include "game_sprite.hpp"
#include "game_object.hpp"
#include "game_skybox.hpp"
#include "gl_err_log.hpp"

#include <btBulletDynamicsCommon.h>

//// Constructor/Destructor {{{
GameSkybox::GameSkybox()
{
        mass = 0;
        sizePercent = 1.f;
}

GameSkybox::GameSkybox(GameSprite *sprite_arg)
{
        sprite = sprite_arg;
        mass = 0;
        sizePercent = 1.f;
}

GameSkybox::GameSkybox(int mass_arg, GameSprite *sprite_arg)
{
        sprite = sprite_arg;
        mass = mass_arg;
        sizePercent = 1.f;
}

GameSkybox::~GameSkybox()
{
        delete motionState;
        delete rigidBody->getMotionState();
        delete rigidBody;
        delete boundingShape;
}
// }}}
