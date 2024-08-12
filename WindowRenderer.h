#ifndef WINDOWRENDERER_H
#define WINDOWRENDERER_H

#include "World.h"
#include "Player.h"
#include "./src/include/SDL2/SDL.h"

class WindowRenderer
{
private:
    bool *Discovered, debug;
    int Width, Height;
    int TileLength;
    float xOffset, yOffset;
    World *_World;
    Player *_Player;
    void Init_Window(const char *title);
    void Init_Renderer();
    void DrawPlayer();
    void DrawSelectedTile(int tileLength, int xRem);
    void DrawPlayerBoundingBox(int tileLength, int xRem);
    void DrawPlayerCollisionBox(int tileLength, int xRem);
    void DrawPlayerVector(int tileLength, int xRem);

public:
    SDL_Window *Window;
    SDL_Renderer *Renderer;
    int MouseX, MouseY;
    WindowRenderer();
    WindowRenderer(World *world, Player *player, int width, int height);
    ~WindowRenderer();
    void Reveal();
    void Discover(int index);
    void RenderFrame();
    void ClearFrame();
    void ToggleDebug();
};
#endif