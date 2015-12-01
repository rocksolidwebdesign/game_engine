#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::_V2::steady_clock;
using std::chrono::_V2::high_resolution_clock;

#include <string>
#include <vector>
#include <map>

using std::string;

#include <GL/glew.h>

#include <GL/glfw.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "../common/shader.hpp"
#include "../common/texture.hpp"
#include "../common/objloader.hpp"

#include "game_engine.hpp"
#include "game_sprite.hpp"
#include "game_object.hpp"
#include "game_skybox.hpp"
#include "game_player.hpp"
#include "game_shader.hpp"
#include "game_camera.hpp"
#include "game_physics.hpp"
#include "gl_err_log.hpp"

#include <btBulletDynamicsCommon.h>

// TODO
//
// Escape Velocity Clone
// Myth: The Fallen Lords Clone
// Total Annihilation Clone
// Descent Clone
// Cosmic Rift Clone
//
// 1. Change front face culling when drawing skybox only
// 2. Proove rotations with quaternions, basic look controls + roll
// 2. Manage player with physics
// 3. Connect the directions correctly to the mouse
// 4. Blender get it to make things right side up, or set GL to have Z be up
// 5. Setup acceleration type movement instead of just forward backward
// 6. Make a space ship model
// 7. Investigate Lua
// 8. Build a chessboard grid
//      turn the plane into a cube that the other cube can fall off of
//      make a multidimensional array
//      hydrate grid tile objects from the array
//      add grid tile objects to the system for rendering and physics processing
// 9. Build walls on the grid

// Constructor/Destructor {{{
GameEngine::GameEngine(string windowTitle_arg)
{
        //fprintf( stderr, "Game started\n" );

        windowTitle = windowTitle_arg;
        windowWidth = 1600;
        windowHeight = 900;
        mouseSpeed = 1.0f;
        movementSpeed = 100.0f;

        //fprintf( stderr, "running main game loop...\n" );
}

GameEngine::~GameEngine()
{
        delete shader;
        delete camera;
        delete skybox;
        delete player;
        delete physics;
        //delete objects.at(0);
        //delete objects.at(1);
        //delete sprites.at("cube");
        //delete sprites.at("ground");
        //delete sprites.at("skybox");
        //delete sprites.at("ship");
}
// }}}

// Initialization {{{
GameEngine* GameEngine::initGame(void)
{
        initGlfw();
        openWindow();
        initGlew();

        initCamera();
        initShader();
        initPhysics();

        loadSprites();
        loadObjects();

        initPlayer();
        initSkybox();

        return this;
}

GameEngine* GameEngine::initGlfw(void)
{
        //fprintf( stderr, "Initializing GLFW\n" );

        // Initialise GLFW
        if( !glfwInit() )
        {
                //fprintf( stderr, "Failed to initialize GLFW\n" );
                // TODO: turn these old error returns into exceptions
                //return -1;
        }

        return this;
}

GameEngine* GameEngine::initGlew(void)
{
        fprintf( stderr, "Initializing GLEW\n" );

        // Initialize GLEW
        glewExperimental = true; // Needed for core profile
        if( glewInit() != GLEW_OK )
        {
                fprintf(stderr, "Failed to initialize GLEW\n");
                //return -1;
        }

        return this;
}

GameEngine* GameEngine::openWindow(void)
{
        //fprintf( stderr, "Opening Window\n" );

        GLERR(glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4));
        GLERR(glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3));
        GLERR(glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3));
        GLERR(glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE));

        // Open a window and create its OpenGL context
        if( !glfwOpenWindow( windowWidth, windowHeight, 0,0,0,0, 32,0, GLFW_WINDOW ) )
        {
                //fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
                glfwTerminate();
                //return -1;
        }

        GLERR(glfwEnable(GLFW_STICKY_KEYS));// Ensure we can capture the escape key being pressed below
        GLERR(glEnable(GL_DEPTH_TEST)); // Enable depth test
        GLERR(glDepthFunc(GL_LESS)); // Accept fragment if it closer to the camera than the former one

        clearBackgroundColor();

        GLERR(glfwSetWindowTitle( windowTitle.c_str() ));

        return this;
}

GameEngine* GameEngine::clearBackgroundColor(void)
{
        //fprintf( stderr, "Clearing background to DARK_BLUE\n" );

        // dark blue background
        GLERR(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
        //GLERR(glDisable(GL_CULL_FACE));
        //GLERR(glEnable(GL_CULL_FACE));

        return this;
}

GameEngine* GameEngine::initShader(void)
{
        shader = new GameShader();
        shader->init(camera);

        return this;
}

GameEngine* GameEngine::initPhysics(void)
{
        physics = new GamePhysics();
        physics->init();

        return this;
}

GameEngine* GameEngine::initCamera(void)
{
        camera = new GameCamera();
        camera->init();

        return this;
}
// }}}

// Load Sprites {{{
GameEngine* GameEngine::loadSprites(void)
{
        loadGroundSprite();
        loadCubeSprite();
        loadSkyboxSprite();
        loadShipSprite();

        return this;
}

GameEngine* GameEngine::loadObjects(void)
{
        loadGroundObject();
        loadCubeObject();
        loadShipObject();

        return this;
}

//GameEngine* GameEngine::loadWhiteMapTile(void)
//{
//        string name = "ground";
//        string meshFile = "meshes/ground.obj";
//        string textureFile = "textures/grass.bmp";
//
//        int numTriangles = 2;
//        GameSprite *loadedSprite = new GameSprite(numTriangles, meshFile, textureFile);
//        loadedSprite->init();
//
//        sprites.insert(std::pair<string,GameSprite*>(name, loadedSprite));
//}

GameEngine* GameEngine::loadGroundSprite(void)
{
        GameSprite *loadedGroundSprite;

        string name = "ground";
        string meshFile = "meshes/ground.obj";
        string textureFile = "textures/grass.tga";

        int numTriangles = 2;
        loadedGroundSprite = new GameSprite(numTriangles, meshFile, textureFile);
        loadedGroundSprite->init();

        sprites.insert(std::pair<string,GameSprite*>(name, loadedGroundSprite));

        return this;
}

GameEngine* GameEngine::loadCubeSprite(void)
{
        GameSprite *loadedCubeSprite;

        //string name = "cube";
        //string meshFile = "meshes/cube.obj";
        //string textureFile = "textures/brushed_metal.tga";
        string name = "cube";
        string meshFile = "meshes/skybox.obj";
        string textureFile = "textures/space_skybox_by_Orindoomhammer_black.tga";

        int numTriangles = 12;
        loadedCubeSprite = new GameSprite(numTriangles, meshFile, textureFile);
        loadedCubeSprite->init();

        sprites.insert(std::pair<string,GameSprite*>(name, loadedCubeSprite));

        return this;
}

GameEngine* GameEngine::loadShipSprite(void)
{
        GameSprite *loadedCubeSprite;

        string name = "ship";
        string meshFile = "meshes/ship.obj";
        string textureFile = "textures/spacecraft.tga";

        int numTriangles = 2064;
        loadedCubeSprite = new GameSprite(numTriangles, meshFile, textureFile);
        loadedCubeSprite->init();

        sprites.insert(std::pair<string,GameSprite*>(name, loadedCubeSprite));

        return this;
}

GameEngine* GameEngine::loadSkyboxSprite(void)
{
        //GameSprite *loadedSkyboxSprite;
        //string name = "skybox";
        //string meshFile = "meshes/skydome.obj";
        //string textureFile = "textures/redband_skydome.tga";
        ////string textureFile = "textures/hubble_huge.bmp";
        ////string textureFile = "textures/milky_way.bmp";
        ////string textureFile = "textures/earth_map.bmp";

        //int numTriangles = 960;
        //loadedSkyboxSprite = new GameSprite(numTriangles, meshFile, textureFile);
        //loadedSkyboxSprite->init();

        //sprites.insert(std::pair<string,GameSprite*>(name, loadedSkyboxSprite));
        GameSprite *loadedSkyboxSprite;
        string name = "skybox";
        string meshFile = "meshes/skydome.obj";
        string textureFile = "textures/redband_skydome.tga";
        //string textureFile = "textures/hubble_huge.bmp";
        //string textureFile = "textures/milky_way.bmp";
        //string textureFile = "textures/earth_map.bmp";

        int numTriangles = 960;
        loadedSkyboxSprite = new GameSprite(numTriangles, meshFile, textureFile);
        loadedSkyboxSprite->init();

        sprites.insert(std::pair<string,GameSprite*>(name, loadedSkyboxSprite));

        return this;
}

GameEngine* GameEngine::loadGroundObject(void)
{
        // initial position
        float x =   0.0f;
        float y =  -5.0f;
        float z =   0.0f;
        //float scaling = 10.0f;
        float scaling = 1.0f;

        int mass = 0;

        string spriteName = "ground";
        GameSprite *groundSprite = sprites.at(spriteName);
        GameObject *ground = new GameObject(mass, groundSprite);

        ground->setCoordX(x);
        ground->setCoordY(y);
        ground->setCoordZ(z);
        ground->setScaling(scaling);

        ground->loadPhysics();

        physics->getWorld()->addRigidBody(ground->getRigidBody());

        objects.push_back(ground);

        return this;
}

GameEngine* GameEngine::loadShipObject(void)
{
        // initial position
        float x =   0.0f;
        float y =  30.0f;
        float z =   0.0f;
        //float scaling = 10.0f;
        float scaling = 2.0f;

        int mass = 1;

        string spriteName = "ship";
        GameSprite *shipSprite = sprites.at(spriteName);
        GameObject *ship = new GameObject(mass, shipSprite);

        ship->setCoordX(x);
        ship->setCoordY(y);
        ship->setCoordZ(z);
        ship->setScaling(scaling);

        ship->loadPhysics();

        physics->getWorld()->addRigidBody(ship->getRigidBody());
        ship->getRigidBody()->setAngularVelocity(btVector3(5.f,7.f,6.f));

        objects.push_back(ship);

        return this;
}

GameEngine* GameEngine::loadCubeObject(void)
{
        // initial position
        float x =   0.0f;
        float y =  30.0f;
        float z =   0.0f;

        int mass = 1;

        string spriteName = "cube";
        GameSprite *cubeSprite = sprites.at(spriteName);
        GameObject *cube = new GameObject(mass, cubeSprite);

        cube->setCoordX(x);
        cube->setCoordY(y);
        cube->setCoordZ(z);
        cube->loadPhysics();

        physics->getWorld()->addRigidBody(cube->getRigidBody());
        cube->getRigidBody()->setAngularVelocity(btVector3(8.f,0.f,2.f));

        objects.push_back(cube);

        return this;
}

GameEngine* GameEngine::initSkybox(void)
{
        // initial position
        float x =   3.0f;
        float y =   15.0f;
        float z =   25.0f;
        float scaling = 999.0f;

        int mass = 0;

        string spriteName = "skybox";
        GameSprite *skyboxSprite = sprites.at(spriteName);
        skybox = new GameObject(skyboxSprite);

        skybox->setCoordX(x);
        skybox->setCoordY(y);
        skybox->setCoordZ(z);

        skybox->setScaling(scaling);

        return this;
}

GameEngine* GameEngine::initPlayer(void)
{
        // initial position
        float x =   10.0f;
        float y =   5.0f;
        float z =   -10.0f;

        int mass = 0;

        player = new GameObject(mass);

        player->setCoordX(x);
        player->setCoordY(y);
        player->setCoordZ(z);
        player->setHorizontalAngle(5.66f);
        player->setVerticalAngle(-0.449670f);

        //player->loadPhysics();

        snapCameraToPlayer();

        return this;
}

GameEngine* GameEngine::snapCameraToPlayer(void)
{
        // initial position
        camera->setCoordX(player->getCoordX());
        camera->setCoordY(player->getCoordY());
        camera->setCoordZ(player->getCoordZ());
        camera->setHorizontalAngle(player->getHorizontalAngle());
        camera->setVerticalAngle(player->getVerticalAngle());

        //snapSkyboxToCamera();

        return this;
}

GameEngine* GameEngine::snapSkyboxToCamera(void)
{
        // initial position
        skybox->setCoordX(camera->getCoordX());
        skybox->setCoordY(camera->getCoordY());
        skybox->setCoordZ(camera->getCoordZ());

        return this;
}
// }}}

// Main Game Loop {{{
GameEngine* GameEngine::run(void)
{
        fprintf(stderr, "run: init\n");
        initGame();

        fprintf(stderr, "run: loop\n");

        // reset the position for the next frame
        GLERR(glfwSetMousePos(windowWidth/2, windowHeight/2));

        do
        {
                timerBegunAt = steady_clock::now();
                processFrame();
                timerEndedAt = steady_clock::now();

                processInput();
        }
        while( keepGoing() );

        fprintf(stderr, "run: quit\n");
        GLERR(glfwTerminate());

        return this;
}

GameEngine* GameEngine::processInput(void)
{
        int xpos, ypos;
        glfwGetMousePos(&xpos, &ypos);

        // reset the position for the next frame
        glfwSetMousePos(windowWidth/2, windowHeight/2);

        // Move forward
        float deltaTime = duration_cast<milliseconds>(timerEndedAt - timerBegunAt).count() / 10000.f;

        float verticalIncrease;
        float horizontalIncrease;

        if( (windowHeight/2 - ypos) != 0 )
        {
                verticalIncrease = mouseSpeed * deltaTime * float(windowHeight/2 - ypos);
                player->adjustVerticalAngle(verticalIncrease * -1.0f);
        }

        if( (windowWidth/2 - xpos) != 0 )
        {
                horizontalIncrease = mouseSpeed * deltaTime * float(windowWidth/2  - xpos);
                player->adjustHorizontalAngle(horizontalIncrease);
        }

        float movementDistance = 0.f;

        //if( glfwGetKey(GLFW_KEY_UP   ) == GLFW_PRESS ) { player->adjustCoordZ((movementDistance = deltaTime * movementSpeed)       ); }
        //if( glfwGetKey(GLFW_KEY_DOWN ) == GLFW_PRESS ) { player->adjustCoordZ((movementDistance = deltaTime * movementSpeed) * -1.f); }
        //if( glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS ) { player->adjustCoordX((movementDistance = deltaTime * movementSpeed) * -1.f); }
        //if( glfwGetKey(GLFW_KEY_LEFT ) == GLFW_PRESS ) { player->adjustCoordX((movementDistance = deltaTime * movementSpeed)       ); }
        //
        //if( verticalIncrease > 0.f || horizontalIncrease > 0.f || movementDistance > 0.f )
        //{
        //        fprintf(stderr, "frame render time: %f\n", deltaTime);
        //        fprintf(stderr, "vertical motion: %f\n", verticalIncrease);
        //        fprintf(stderr, "horizontal motion: %f\n", horizontalIncrease);
        //        fprintf(stderr, "movement distance: %f\n", movementDistance);
        //}
}

GameEngine* GameEngine::processFrame(void)
{
        shader->newFrame();

        physics->stepSimulation();

        processObjects();
        processPlayer(); // this will snap the camera to the player
        processSkybox(); // and this will snap the skybox to the camera

        shader->reset();
}

GameEngine* GameEngine::processPlayer(void)
{
        // right now this is being done without physics
        // and just via raw input
        //player->processPhysics();

        snapCameraToPlayer();
}

GameEngine* GameEngine::processSkybox(void)
{
        snapSkyboxToCamera();

        shader->draw(skybox);
}

GameEngine* GameEngine::processObjects(void)
{
        for ( auto &worldObject : objects )
        {
                shader->draw(worldObject);

                worldObject->processPhysics();
        }
}

bool GameEngine::keepGoing(void)
{
        // Check if the ESC key was pressed or the window was closed
        return glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS && glfwGetWindowParam( GLFW_OPENED );
}
// }}}
