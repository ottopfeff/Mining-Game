#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include <stdio.h>
#include "World.h"
#include "./src/include/SDL2/SDL.h"

typedef enum direction_t
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    UPLEFT,
    UPRIGHT,
    DOWNLEFT,
    DOWNRIGHT,
    NONE
} direction;

class Player
{
private:
    World* _World;
public:
    float x, y;
    direction dir;
    int score;
    Player(World* World);
    ~Player();
    void TryMove(direction dir);
};

#endif

