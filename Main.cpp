#include "World.h"
#include "Player.h"
#include "ConsoleRenderer.h"
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
    SDL_Init(SDL_INIT_EVERYTHING);
    srand(time(0));
    World world(40, 40, 30, 15, 100);
    Player player(&world);

    WindowRenderer renderer(&world, &player, 800, 600);
    // ConsoleRenderer renderer(&world, &player, 800, 600);
    InputManager inputManager(&world, &player, &renderer);

    // renderer.Discover(player.x + (player.y * world.Width));

    int64_t time = now();

    SDL_Event window_event;

    while (1)
    {
        if (now() - time >= 50)
        {

            time = now();
                if (SDL_PollEvent(&window_event))
                {
                    if (window_event.type == SDL_QUIT)
                    {
                        break;
                    }
                }
            renderer.ClearFrame();
            renderer.RenderFrame();
            inputManager.ManageInput();
        }
    }
    // SDL_Event window_event;
    // while (1)
    // {

    // }
}