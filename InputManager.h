#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include "World.h"
#include "Player.h"
#include "WindowRenderer.h"
#include <stdio.h>
#include "./src/include/SDL2/SDL.h"

class InputManager
{
private:
    World *_World;
    Player *_Player;
    WindowRenderer *_Renderer;
    const Uint8* keys;

public:
    InputManager();
    InputManager(World *world, Player *player, WindowRenderer *renderer);
    ~InputManager();
    void ManageInput();
};
#endif
