#include <iostream>
#include <time.h>
#include "Game.h"

int main()
{
    //Seeding
    srand(static_cast<unsigned int>(time(NULL)));

    Game* game = new Game();
    game->startGame();
    delete game;
}
