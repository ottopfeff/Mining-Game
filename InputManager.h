#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include "World.h"
#include "Player.h"
#include "Renderer.h"
#include <windows.h>

class InputManager
{
private:
    World *_World;
    Player *_Player;
    Renderer *_Renderer;

public:
    InputManager();
    InputManager(World *world, Player *player, Renderer *renderer);
    ~InputManager();
    void ManageInput();
};
#endif
