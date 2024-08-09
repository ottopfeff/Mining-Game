#include "InputManager.h"

InputManager::InputManager(World *world, Player *player, WindowRenderer *renderer)
{
    _World = world;
    _Player = player;
    _Renderer = renderer;
    keys = SDL_GetKeyboardState(NULL);
}

InputManager::InputManager()
    : _World(), _Player() {}

InputManager::~InputManager()
{
}

void InputManager::ManageInput()
{
    SDL_PumpEvents();
    SDL_GetMouseState(&(_Renderer->MouseX), &(_Renderer->MouseY));
    direction move_dir = NONE;
    if (keys[SDL_SCANCODE_W] && !(keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_D]))
        move_dir = UP;
    else if (keys[SDL_SCANCODE_S] && !(keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_D]))
        move_dir = DOWN;
    else if (keys[SDL_SCANCODE_A] && !(keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_S]))
        move_dir = LEFT;
    else if (keys[SDL_SCANCODE_D] && !(keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_S]))
        move_dir = RIGHT;
    else if (keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_A])
        move_dir = UPLEFT;
    else if (keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_D])
        move_dir = UPRIGHT;
    if (keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_A])
        move_dir = DOWNLEFT;
    else if (keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_D])
        move_dir = DOWNRIGHT;
    _Player->TryMove(move_dir);
    if (keys[SDL_SCANCODE_TAB])
        _Renderer->Reveal();
    if (keys[SDL_SCANCODE_F1])
        _Renderer->ToggleDebug();

    if (keys[SDL_SCANCODE_SPACE])
    {
        // int x = _Player->x;
        // int y = _Player->y;
        // switch (_Player->dir)
        // {
        // case LEFT:
        //     x--;
        //     break;
        // case RIGHT:
        //     x++;
        //     break;
        // case UP:
        //     y--;
        //     break;
        // case DOWN:
        //     y++;
        //     break;
        // }
        // _Player->score += _World->DestroyTile(x, y);
        // //_Renderer->Discover(x + (y * _World->Width));
    }

    if (keys[SDL_SCANCODE_ESCAPE])
        exit(0);
}
