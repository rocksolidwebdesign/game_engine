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

#include "game_player.hpp"
#include "gl_err_log.hpp"

#include <btBulletDynamicsCommon.h>

// Constructor/Destructor {{{
GamePlayer::GamePlayer()
{
        mass = 1;
        sizePercent = 1.f;
        horizontalAngle = 0.f;
        //horizontalAngle = 3.14f;
        verticalAngle   = -0.449670f;

        x = 0.f;
        y = 0.f;
        z = 0.f;
}

GamePlayer::GamePlayer(int mass_arg)
{
        mass = mass_arg;
        sizePercent = 1.f;
        horizontalAngle = 0.f;
        //horizontalAngle = 3.14f;
        verticalAngle   = -0.449670f;

        x = 0.f;
        y = 0.f;
        z = 0.f;
}

GamePlayer::~GamePlayer()
{
        delete motionState;
        delete rigidBody->getMotionState();
        delete rigidBody;
        delete boundingShape;
}
// }}}

GamePlayer* GamePlayer::loadPhysics(void)
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

GamePlayer* GamePlayer::loadFallRigidBody(void)
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

GamePlayer* GamePlayer::loadGroundRigidBody(void)
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

glm::mat4 GamePlayer::getPosition()
{
        return glm::translate(glm::mat4(1.0f), glm::vec3(
                                x,
                                y,
                                z
                                ));
}

GamePlayer* GamePlayer::setScaling(float newScale)
{
        sizePercent = newScale;

        return this;
}

float GamePlayer::getCoordX()
{
        return x;
}

float GamePlayer::getCoordY()
{
        return y;
}

float GamePlayer::getCoordZ()
{
        return z;
}

btRigidBody* GamePlayer::getRigidBody()
{
        return rigidBody;
}

GamePlayer* GamePlayer::processPhysics()
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

GamePlayer* GamePlayer::setCoordX(float x_arg)
{
        x = x_arg;

        return this;
}

GamePlayer* GamePlayer::setCoordY(float y_arg)
{
        y = y_arg;

        return this;
}

GamePlayer* GamePlayer::setCoordZ(float z_arg)
{
        z = z_arg;

        return this;
}

GamePlayer* GamePlayer::setRotation(btQuaternion q)
{
        float angle = q.getAngle();
        btVector3 bt_axis = q.getAxis();

        glm::vec3 axis(
                        bt_axis.getX(),
                        bt_axis.getY(),
                        bt_axis.getZ()
                      );

        rotation = glm::angleAxis(glm::degrees(angle), axis);

        return this;
}

glm::mat4 GamePlayer::getScaling()
{
        return glm::scale(sizePercent, sizePercent, sizePercent);
}

glm::mat4 GamePlayer::getRotation()
{
        return glm::toMat4(rotation);
}

int GamePlayer::getNumTriangles()
{
        return 0;
}

GLuint* GamePlayer::getTexture()
{
        return sprite->getTexture();
}

GLuint* GamePlayer::getVertexBuffer()
{
        return sprite->getVertexBuffer();
}

GLuint* GamePlayer::getUvBuffer()
{
        return sprite->getUvBuffer();
}

GamePlayer* GamePlayer::adjustVerticalAngle(float increase)
{
        verticalAngle += increase;

        fprintf(stderr, "new vertical angle: %f\n", verticalAngle);

        return this;
}

GamePlayer* GamePlayer::adjustHorizontalAngle(float increase)
{
        horizontalAngle += increase;

        fprintf(stderr, "new horizontal angle: %f\n", horizontalAngle);

        return this;
}

float GamePlayer::getHorizontalAngle()
{
        fprintf(stderr, "GET PLAYER horizontalAngle angle: %f\n", horizontalAngle);
        
        return horizontalAngle;
}

float GamePlayer::getVerticalAngle()
{
        fprintf(stderr, "GET PLAYER verticalAngle angle: %f\n", verticalAngle);
        
        return verticalAngle;
}

GamePlayer* GamePlayer::setVerticalAngle(float angle)
{
        verticalAngle = angle;

        fprintf(stderr, "new vertical angle: %f\n", verticalAngle);

        return this;
}

GamePlayer* GamePlayer::setHorizontalAngle(float angle)
{
        horizontalAngle = angle;

        fprintf(stderr, "new horizontal angle: %f\n", horizontalAngle);

        return this;
}

GamePlayer* GamePlayer::adjustCoordX(float x_arg)
{
        x += x_arg;

        return this;
}

GamePlayer* GamePlayer::adjustCoordY(float y_arg)
{
        y += y_arg;

        return this;
}

GamePlayer* GamePlayer::adjustCoordZ(float z_arg)
{
        z += z_arg;

        return this;
}