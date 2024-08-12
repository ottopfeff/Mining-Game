#include "Player.h"
#include <stdio.h>

#define SQRT2INV 0.7071f; //diagonal movements
#define EPSILON 0.001f; //collision tolerance due to floating point precision errors: collisions to the right will not work if you don't use epsilon
Player::Player(World *world)
{
    this->_World = world;
    this->x = _World->Width / 2;
    this->y = _World->Height / 2;
    speed = 0.4f;               //tiles per tick, 0 < speed < 1;
    size = 0.6f;                //0 < size < 1;
    dir = NONE;
    score = 0;
    canMine = false;
    reach = 2.0f;

}

Player::~Player()
{
}

void Player::TryMove(direction dir)
{
    Vec2 velocity;
    this->dir = dir;
    switch (dir)
    {
    case LEFT:
        velocity = {-speed, 0};
        break;
    case RIGHT:
        velocity = {speed, 0};
        break;
    case UP:
        velocity = {0, -speed};
        break;
    case DOWN:
        velocity = {0, speed};
        break;
    case UPLEFT:
        velocity = {-speed * 0.7071f, -speed * 0.7071f};
        break;
    case UPRIGHT:
        velocity = {speed * 0.7071f, -speed * 0.7071f};
        break;
    case DOWNLEFT:
        velocity = {-speed * 0.7071f, speed * 0.7071f};
        break;
    case DOWNRIGHT:
        velocity = {speed * 0.7071f, speed * 0.7071f};
        break;
    }
    bool NW, NE, SW, SE;
    int xStart, yStart, xEnd, yEnd;

    if (velocity.x != 0)
    {
        x += velocity.x;

        xStart = (int)SDL_floorf(x);
        yStart = (int)SDL_floorf(y);
        xEnd = (int)SDL_floorf(x + size);
        yEnd = (int)SDL_floorf(y + size);

        if (xStart < 0)
        {
            x = EPSILON;
        }
        else if (xEnd > _World->Width - 1)
        {
            x = _World->Width - size - EPSILON;
        }
        else
        {

            NW = _World->tiles[xStart][yStart] != AIR;
            NE = _World->tiles[xEnd][yStart] != AIR;
            SW = _World->tiles[xStart][yEnd] != AIR;
            SE = _World->tiles[xEnd][yEnd] != AIR;

            if (NW || SW)
            {
                x = (float)xStart + 1 + EPSILON;
            }
            else if (NE || SE)
            {
                x = (float)xEnd - size - EPSILON;
            }
        }
    }

    if (velocity.y != 0)
    {
        y += velocity.y;

        xStart = (int)SDL_floorf(x);
        yStart = (int)SDL_floorf(y);
        xEnd = (int)SDL_floorf(x + size);
        yEnd = (int)SDL_floorf(y + size);

        if (yStart < 0)
        {
            y = EPSILON;
        }
        else if (yEnd > _World->Height - 1)
        {
            y = _World->Height - size - EPSILON;
        }
        else
        {
            NW = _World->tiles[xStart][yStart] != AIR;
            NE = _World->tiles[xEnd][yStart] != AIR;
            SW = _World->tiles[xStart][yEnd] != AIR;
            SE = _World->tiles[xEnd][yEnd] != AIR;

            if (NW || NE)
            {
                y = (float)yStart + 1 + EPSILON;
            }
            else if (SW || SE)
            {
                y = (float)yEnd - size - EPSILON;
            }
        }
    }
}

bool Collide(BoundingBox b0, BoundingBox b1)
{
    if (b0.x + b0.width < b1.x || b1.x + b1.width < b0.x)
        return true;
    if (b0.y + b0.height < b1.y || b1.y + b1.height < b0.y)
        return true;
    return false;
}