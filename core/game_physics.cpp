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

#include "game_physics.hpp"

#include <btBulletDynamicsCommon.h>

// Constructor/Destructor {{{
GamePhysics::GamePhysics()
{
        grainSize = 1/60.f;
        stepSize = 10;

        broadphase = new btDbvtBroadphase();

        collisionConfiguration = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(collisionConfiguration);

        solver = new btSequentialImpulseConstraintSolver;

        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

        dynamicsWorld->setGravity(btVector3(0,-10,0));
}

GamePhysics::~GamePhysics()
{
        delete broadphase;
        delete collisionConfiguration;
        delete dispatcher;
        delete solver;
        delete dynamicsWorld;
}
// }}}

GamePhysics* GamePhysics::init(void)
{
        return this;
}

GamePhysics* GamePhysics::stepSimulation(void)
{
        getWorld()->stepSimulation(grainSize, stepSize);

        return this;
}

btDiscreteDynamicsWorld* GamePhysics::getWorld(void)
{
        return dynamicsWorld;
}
// }}}
