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
#include "gl_err_log.hpp"

#include <btBulletDynamicsCommon.h>

// Constructor/Destructor {{{
GameObject::GameObject()
{
        mass = 0;
        sizePercent = 1.f;

        horizontalAngle = 0.f;
        verticalAngle   = 0.f;
}

GameObject::GameObject(int mass_arg)
{
        mass = mass_arg;
        sizePercent = 1.f;

        horizontalAngle = 0.f;
        verticalAngle   = 0.f;
}

GameObject::GameObject(GameSprite *sprite_arg)
{
        sprite = sprite_arg;
        mass = 0;
        sizePercent = 1.f;

        horizontalAngle = 0.f;
        verticalAngle   = 0.f;
}

GameObject::GameObject(int mass_arg, GameSprite *sprite_arg)
{
        sprite = sprite_arg;
        mass = mass_arg;
        sizePercent = 1.f;

        horizontalAngle = 0.f;
        verticalAngle   = 0.f;
}

GameObject::~GameObject()
{
        delete motionState;
        delete rigidBody->getMotionState();
        delete rigidBody;
        delete boundingShape;
}
// }}}

GameObject* GameObject::loadPhysics(void)
{
        motionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(x,y,z)));

        if (mass > 0)
        {
                loadFallRigidBody();
        }
        else
        {
                loadGroundRigidBody();
        }

        return this;
}

GameObject* GameObject::loadFallRigidBody(void)
{
        fprintf( stderr, "we have a sphere\n" );
        boundingShape = new btSphereShape(1);

        boundingShape->calculateLocalInertia(mass, motionInertia);
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(
                        mass,
                        motionState,
                        boundingShape,
                        motionInertia
                        );

        rigidBody = new btRigidBody(fallRigidBodyCI);

        return this;
}

GameObject* GameObject::loadGroundRigidBody(void)
{
        fprintf( stderr, "we have a plane\n" );
        boundingShape = new btStaticPlaneShape(btVector3(0,1,0),1);

        int infiniteMass = 0;
        btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(
                        infiniteMass,
                        motionState,
                        boundingShape,
                        motionInertia
                        );

        rigidBody = new btRigidBody(groundRigidBodyCI);

        return this;
}

mat4 GameObject::getPosition()
{
        return translate(mat4(1.0f), vec3(
                                x,
                                y,
                                z
                                ));
}

GameObject* GameObject::setScaling(float newScale)
{
        sizePercent = newScale;

        return this;
}

float GameObject::getCoordX()
{
        return x;
}

float GameObject::getCoordY()
{
        return y;
}

float GameObject::getCoordZ()
{
        return z;
}

btRigidBody* GameObject::getRigidBody()
{
        return rigidBody;
}

GameObject* GameObject::processPhysics()
{
        if (mass > 0) {
                btTransform trans;
                getRigidBody()->getMotionState()->getWorldTransform(trans);

                setRotation(trans.getRotation());

                setCoordX(trans.getOrigin().getX());
                setCoordY(trans.getOrigin().getY());
                setCoordZ(trans.getOrigin().getZ());
        }

        return this;
}

GameObject* GameObject::setCoordX(float x_arg)
{
        x = x_arg;

        return this;
}

GameObject* GameObject::setCoordY(float y_arg)
{
        y = y_arg;

        return this;
}

GameObject* GameObject::setCoordZ(float z_arg)
{
        z = z_arg;

        return this;
}

GameObject* GameObject::setRotation(btQuaternion q)
{
        float angle = q.getAngle();
        btVector3 bt_axis = q.getAxis();

        vec3 axis(
                        bt_axis.getX(),
                        bt_axis.getY(),
                        bt_axis.getZ()
                      );

        rotation = angleAxis(degrees(angle), axis);

        return this;
}

mat4 GameObject::getScaling()
{
        return scale(sizePercent, sizePercent, sizePercent);
}

mat4 GameObject::getRotation()
{
        return toMat4(rotation);
}

int GameObject::getNumTriangles()
{
        return sprite->getNumTriangles();
}

GLuint* GameObject::getTexture()
{
        return sprite->getTexture();
}

GLuint* GameObject::getVertexBuffer()
{
        return sprite->getVertexBuffer();
}

GLuint* GameObject::getUvBuffer()
{
        return sprite->getUvBuffer();
}

GameObject* GameObject::adjustVerticalAngle(float increase)
{
        verticalAngle += increase;

        fprintf(stderr, "new vertical angle: %f\n", verticalAngle);

        return this;
}

GameObject* GameObject::adjustHorizontalAngle(float increase)
{
        horizontalAngle += increase;

        fprintf(stderr, "new horizontal angle: %f\n", horizontalAngle);

        return this;
}

float GameObject::getHorizontalAngle()
{
        return horizontalAngle;
}

float GameObject::getVerticalAngle()
{
        return verticalAngle;
}

GameObject* GameObject::setVerticalAngle(float angle)
{
        verticalAngle = angle;

        fprintf(stderr, "new vertical angle: %f\n", verticalAngle);

        return this;
}

GameObject* GameObject::setHorizontalAngle(float angle)
{
        horizontalAngle = angle;

        fprintf(stderr, "new horizontal angle: %f\n", horizontalAngle);

        return this;
}

GameObject* GameObject::adjustCoordX(float x_arg)
{
        x += x_arg;

        return this;
}

GameObject* GameObject::adjustCoordY(float y_arg)
{
        y += y_arg;

        return this;
}

GameObject* GameObject::adjustCoordZ(float z_arg)
{
        z += z_arg;

        return this;
}

