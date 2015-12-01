#ifndef GAME_PHYSICS_H
#define GAME_PHYSICS_H

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "game_object.hpp"

class GamePhysics
{
        btBroadphaseInterface* broadphase;
        btDefaultCollisionConfiguration* collisionConfiguration;
        btCollisionDispatcher* dispatcher;
        btSequentialImpulseConstraintSolver* solver;
        btDiscreteDynamicsWorld* dynamicsWorld;
	
	float grainSize;
	float stepSize;
	
public:

        GamePhysics();
        ~GamePhysics();

        GamePhysics* init(void);
        GamePhysics* stepSimulation(void);
        btDiscreteDynamicsWorld* getWorld(void);
};

#endif
