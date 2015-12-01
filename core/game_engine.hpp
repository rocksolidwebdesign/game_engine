#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <map>
#include <vector>
#include <string>
#include <chrono>
using std::chrono::time_point;
using std::chrono::_V2::steady_clock;
using std::chrono::_V2::high_resolution_clock;
using std::map;
using std::string;
using std::vector;

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "game_object.hpp"
#include "game_player.hpp"
#include "game_skybox.hpp"
#include "game_camera.hpp"
#include "game_shader.hpp"
#include "game_physics.hpp"

class GameEngine
{
        map<string, GameSprite*> sprites;
        vector<GameObject*> render_objects;
        vector<GameObject*> physics_objects;
        vector<GameObject*> objects;

        float mouseSpeed;
        float movementSpeed;

        GameCamera *camera;
        GameObject *player;
        GameObject *skybox;

        GameShader *shader;
        GamePhysics *physics;

        string windowTitle;
        int windowWidth;
        int windowHeight;

        time_point<steady_clock> timerBegunAt;
        time_point<steady_clock> timerEndedAt;

        GLuint *mVertexArrayID,
               *mProgramID,
               *mMatrixID,
               *mTextureID;

        GameEngine * initGlfw(void);
        GameEngine * initGlew(void);
        GameEngine * openWindow (void);
        GameEngine * clearBackgroundColor(void);
        GameEngine * initGame(void);
        GameEngine * initWindow(void);
        GameEngine * initShader(void);
        GameEngine * initPhysics(void);
        GameEngine * initCamera(void);
        GameEngine * loadSprites(void);
        GameEngine * loadObjects(void);
        GameEngine * loadGroundSprite(void);
        GameEngine * loadCubeSprite(void);
        GameEngine * loadShipSprite(void);
        GameEngine * loadSkyboxSprite(void);
        GameEngine * loadGroundObject(void);
        GameEngine * loadCubeObject(void);
        GameEngine * loadShipObject(void);
        GameEngine * initPlayer(void);
        GameEngine * initSkybox(void);
        GameEngine * snapCameraToPlayer(void);
        GameEngine * snapSkyboxToCamera(void);
        GameEngine * loadTextures(void);
        GameEngine * drawObject(GameObject*);
        GameEngine * drawGround(void);
        GameEngine * drawCube(void);
        GameEngine * processFrame(void);
        GameEngine * processObjects(void);
        GameEngine * processSkybox(void);
        GameEngine * processPlayer(void);
        GameEngine * processInput(void);

        bool keepGoing(void);

        public:

        GameEngine(string);
        ~GameEngine();
        GameEngine * run(void);
};

#endif
