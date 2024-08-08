#ifndef WINDOWRENDERER_H
#define WINDOWRENDERER_H

#define SDL_MAIN_HANDLED

#include "Renderer.h"
#include "World.h"
#include "Player.h"
#include "./src/include/SDL2/SDL.h"

class WindowRenderer : public Renderer
{
private:
    bool *Discovered, debug;
    int Width, Height;
    World *_World;
    Player *_Player;
    

public:
    SDL_Window* Window;
    SDL_Renderer* Renderer;
    WindowRenderer();
    WindowRenderer(World *world, Player *player, int width, int height);
    ~WindowRenderer();
    void Reveal() override;
    void Discover(int index) override;
    void RenderFrame() override;
    void ClearFrame() override;
    void ToggleDebug() override;
    void Init_Window(const char* title);
    void Init_Renderer();
};
#endif