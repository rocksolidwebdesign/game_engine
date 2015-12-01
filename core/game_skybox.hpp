#ifndef GAME_SKYBOX_H
#define GAME_SKYBOX_H

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <btBulletDynamicsCommon.h>

#include "game_sprite.hpp"
#include "game_object.hpp"

class GameSkybox : public GameObject
{
public:
        GameSkybox();
        GameSkybox(GameSprite *sprite_arg);
        GameSkybox(int mass_arg, GameSprite *sprite_arg);
        ~GameSkybox();
};

#endif
