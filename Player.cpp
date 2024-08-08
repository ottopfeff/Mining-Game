#include "Player.h"
Player::Player(World *world)
{
    this->_World = world;
    this->x = _World->Width / 2;
    this->y = _World->Height / 2;
    this->dir = RIGHT;
    this->score = 0;
}

Player::~Player()
{
}

void Player::TryMove(direction direction)
{
    if (this->dir != direction)
        this->dir = direction;
    else
    {
        switch (this->dir)
        {
        case LEFT:
            if (x > 0 && _World->tiles[x - 1][y] == AIR)
                this->x--;
            break;
        case RIGHT:
            if (x < _World->Width - 1 && _World->tiles[x + 1][y] == AIR)
                this->x++;
            break;
        case UP:
            if (y > 0 && _World->tiles[x][y - 1] == AIR)
                this->y--;
            break;
        case DOWN:
            if (y < _World->Height - 1 && _World->tiles[x][y + 1] == AIR)
                this->y++;
            break;
        }
    }
}