#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;

#include <btBulletDynamicsCommon.h>
#include "game_sprite.hpp"

class GamePlayer
{
        GameSprite* sprite;

        btScalar mass;
        btVector3 motionInertia;
        btDefaultMotionState* motionState;
        btRigidBody* rigidBody;
        btCollisionShape* boundingShape;

        mat4 transform;
        quat rotation;

        float horizontalAngle;
        float verticalAngle;

        float sizePercent;

        float x;
        float y;
        float z;

public:
        GamePlayer();
        GamePlayer(int);
        ~GamePlayer();

        GamePlayer* init();
        GamePlayer* loadPhysics(void);
        GamePlayer* loadFallRigidBody(void);
        GamePlayer* loadGroundRigidBody(void);

        mat4 getPosition();
        mat4 getRotation();
        mat4 getScaling();

        GamePlayer* setScaling(float);

        GamePlayer* setCoordX(float);
        GamePlayer* setCoordY(float);
        GamePlayer* setCoordZ(float);

        GamePlayer* setRotation(btQuaternion);

        GamePlayer* adjustVerticalAngle(float);
        GamePlayer* adjustHorizontalAngle(float);
        GamePlayer* setVerticalAngle(float);
        GamePlayer* setHorizontalAngle(float);
        GamePlayer* readjustView(void);

        int getNumTriangles(void);
        GLuint* getVertexBuffer(void);
        GLuint* getUvBuffer(void);
        GLuint* getTexture(void);

        GamePlayer* processPhysics();

        float getCoordX();
        float getCoordY();
        float getCoordZ();
        GamePlayer* adjustCoordX(float);
        GamePlayer* adjustCoordY(float);
        GamePlayer* adjustCoordZ(float);
        float getVerticalAngle();
        float getHorizontalAngle();
        btRigidBody* getRigidBody();
};

#endif
