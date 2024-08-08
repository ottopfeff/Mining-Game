#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include "World.h"

typedef enum direction_t
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NONE
} direction;

class Player
{
private:
    World* _World;
public:
    int x, y;
    direction dir;
    int score;
    Player(World* World);
    ~Player();
    void TryMove(direction direction);
};

#endif

