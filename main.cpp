#include "core/game_engine.hpp"

int main(void)
{
        GameEngine *myEngine = new GameEngine("Space Arena");
        myEngine->run();
        delete myEngine;

        return 0;
}
