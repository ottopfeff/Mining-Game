#include "InputManager.h"\

float Distance(int x0, int y0, int x1, int y1)
{
    float distance = SDL_sqrtf(((x1 - x0) * (x1 - x0)) + ((y1 - y0) * (y1 - y0)));
    printf("DISTANCE: %f\n", distance);
    return distance;
}

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
    int mouseState = SDL_GetMouseState(&(_Renderer->MouseX), &(_Renderer->MouseY));
    if (SDL_BUTTON(mouseState) == 1 && _Player->canMine)
    {
        _Player->score += _World->DestroyTile(_Renderer->MouseWorldX, _Renderer->MouseWorldY);
    }
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
    if (keys[SDL_SCANCODE_ESCAPE])
        exit(0);

    if (Distance(_Player->x, _Player->y, _Renderer->MouseWorldX, _Renderer->MouseWorldY) <= _Player->reach)
        _Player->canMine = true;
    else
        _Player->canMine = false;
}
