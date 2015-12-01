#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;

#include <btBulletDynamicsCommon.h>

#include "game_sprite.hpp"

class GameObject
{
protected:
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
        GameObject();
        GameObject(int);
        GameObject(GameSprite*);
        GameObject(int, GameSprite*);
        ~GameObject();

        GameObject* loadPhysics(void);
        GameObject* loadFallRigidBody(void);
        GameObject* loadGroundRigidBody(void);

        mat4 getPosition();
        mat4 getRotation();
        mat4 getScaling();

        GameObject* adjustVerticalAngle(float);
        GameObject* adjustHorizontalAngle(float);
        GameObject* setVerticalAngle(float);
        GameObject* setHorizontalAngle(float);

        GameObject* setScaling(float);

        GameObject* setCoordX(float);
        GameObject* setCoordY(float);
        GameObject* setCoordZ(float);
        GameObject* setRotation(btQuaternion);

        int getNumTriangles(void);
        GLuint* getVertexBuffer(void);
        GLuint* getUvBuffer(void);
        GLuint* getTexture(void);

        GameObject* processPhysics();

        float getCoordX();
        float getCoordY();
        float getCoordZ();
        GameObject* adjustCoordX(float);
        GameObject* adjustCoordY(float);
        GameObject* adjustCoordZ(float);
        float getHorizontalAngle();
        float getVerticalAngle();
        btRigidBody* getRigidBody();
};

#endif
