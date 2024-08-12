#include "Player.h"
#include <stdio.h>

#define SQRT2INV 0.7071f;
#define EPSILON 0.001f;
Player::Player(World *world)
{
    this->_World = world;
    this->x = _World->Width / 2;
    this->y = _World->Height / 2;
    size = 0.6f;
    velocity = {0, 0};
    this->dir = NONE;
    this->score = 0;
    BoundingBox = {x,
                   y,
                   size,
                   size};
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
        velocity = {-0.3, 0};
        break;
    case RIGHT:
        velocity = {0.3, 0};
        break;
    case UP:
        velocity = {0, -0.3};
        break;
    case DOWN:
        velocity = {0, 0.3};
        break;
    case UPLEFT:
        velocity = {-0.3f * 0.7071f, -0.3 * 0.7071f};
        break;
    case UPRIGHT:
        velocity = {0.3f * 0.7071f, -0.3 * 0.7071f};
        break;
    case DOWNLEFT:
        velocity = {-0.3f * 0.7071f, 0.3 * 0.7071f};
        break;
    case DOWNRIGHT:
        velocity = {0.3f * 0.7071f, 0.3 * 0.7071f};
        break;
    }
    bool NW, NE, SW, SE;

    x += velocity.x;

    int xStart = (int)SDL_floorf(x);
    int yStart = (int)SDL_floorf(y);
    int xEnd = (int)SDL_floorf(x + size);
    int yEnd = (int)SDL_floorf(y + size);

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
    BoundingBox.x = x;
    BoundingBox.y = y;
    velocity = {0, 0};
}

bool Collide(BoundingBox b0, BoundingBox b1)
{
    if (b0.x + b0.width < b1.x || b1.x + b1.width < b0.x)
        return true;
    if (b0.y + b0.height < b1.y || b1.y + b1.height < b0.y)
        return true;
    return false;
}