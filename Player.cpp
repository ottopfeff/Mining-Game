#include "Player.h"
#include <stdio.h>

#define SQRT2INV 0.7071f;
Player::Player(World *world)
{
    this->_World = world;
    this->x = _World->Width / 2;
    this->y = _World->Height / 2;
    this->dir = NONE;
    this->score = 0;
}

Player::~Player()
{
}

void Player::TryMove(direction dir)
{
    switch (dir)
    {
    case LEFT:
        x -= 0.2f;
        break;
    case RIGHT:
        x += 0.2f;
        break;
    case UP:
        y -= 0.2f;
        break;
    case DOWN:
        y += 0.2f;
        break;
    case UPLEFT:
        x -= 0.2f * SQRT2INV;
        y -= 0.2f * SQRT2INV;
        break;
    case UPRIGHT:
        x += 0.2f * SQRT2INV;
        y -= 0.2f * SQRT2INV;
        break;
    case DOWNLEFT:
        x -= 0.2f * SQRT2INV;
        y += 0.2f * SQRT2INV;
        break;
    case DOWNRIGHT:
        x += 0.2f * SQRT2INV;
        y += 0.2f * SQRT2INV;
        break;
    }
}