#define SDL_MAIN_HANDLED

#include "World.h"
#include "Player.h"
#include "WindowRenderer.h"
#include "InputManager.h"
#include <iostream>
#include <windows.h>
#include <chrono>
using namespace std::chrono;

int64_t now()
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    int64_t milliseconds = duration_cast<std::chrono::milliseconds>(duration).count();
    return milliseconds;
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        fprintf(stderr, "Could not initialise SDL: %s\n", SDL_GetError());
        exit(-1);
    }
    srand(time(0));
    World world(200, 200, 200, 15, 0);
    Player player(&world);

    WindowRenderer renderer(&world, &player, 800, 600);
    InputManager inputManager(&world, &player, &renderer);

    // renderer.Discover(player.x + (player.y * world.Width));
    world.tiles[(int)player.x][(int)player.y] = AIR;

    int64_t time = now();

    SDL_Event window_event;

    while (1)
    {
        if (SDL_PollEvent(&window_event))
        {
            if (window_event.type == SDL_QUIT)
            {
                break;
            }
        }
        if (now() - time >= 20)
        {

            time = now();
            renderer.ClearFrame();
            renderer.RenderFrame();
            inputManager.ManageInput();
        }
    }
    SDL_Quit();
}