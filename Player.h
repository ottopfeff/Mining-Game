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

typedef struct BoundingBox_t
{
    float x;
    float y;
    float width;
    float height;
} BoundingBox;

typedef struct Vector2_t {
    float x;
    float y;
} Vec2;

class Player
{
private:
    World *_World;
public:
    float size;
    float x, y;
    direction dir;
    Vec2 velocity;
    int score;
    BoundingBox BoundingBox;

    Player(World *World);
    ~Player();
    void TryMove(direction dir);
};

#endif
