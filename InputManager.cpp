#include "InputManager.h"

InputManager::InputManager(World *world, Player *player, Renderer *renderer)
{
    _World = world;
    _Player = player;
    _Renderer = renderer;
}

InputManager::InputManager()
    : _World(), _Player() {}

InputManager::~InputManager()
{
}

void InputManager::ManageInput()
{
    direction move_dir = NONE;
    if (GetAsyncKeyState(VK_LEFT) && 0x8000)
        move_dir = LEFT;
    else if (GetAsyncKeyState(VK_RIGHT) && 0x8000)
        move_dir = RIGHT;
    else if (GetAsyncKeyState(VK_UP) && 0x8000)
        move_dir = UP;
    else if (GetAsyncKeyState(VK_DOWN) && 0x8000)
        move_dir = DOWN;
    else if (GetAsyncKeyState(VK_TAB) && 0x8000)
        _Renderer->Reveal();
    else if (GetAsyncKeyState(VK_F1) & 0x8000)
        _Renderer->ToggleDebug();
        
    if (move_dir != NONE)
        _Player->TryMove((direction)move_dir);

    if (GetAsyncKeyState(VK_SPACE) && 0x8000)
    {
        int x = _Player->x;
        int y = _Player->y;
        switch (_Player->dir)
        {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        }
        _Player->score += _World->DestroyTile(x, y);
        //_Renderer->Discover(x + (y * _World->Width));
    }

    if (GetAsyncKeyState(VK_ESCAPE) && 0x8000)
        exit(0);
}
