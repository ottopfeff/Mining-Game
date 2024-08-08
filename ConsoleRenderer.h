#ifndef CONSOLERENDERER_H
#define CONSOLERENDERER_H

#include <stdio.h>
#include <cstring>
#include "Renderer.h"
#include "World.h"
#include "Player.h"

class ConsoleRenderer : public Renderer
{
private:
    char *buffer, debug;
    bool *discovered;
    int window_width, window_height;

public:
    World *world;
    Player *player;

    ConsoleRenderer();
    ConsoleRenderer(World *world, Player *player, int width, int height);
    ~ConsoleRenderer();
    void Reveal() override;
    void Discover(int index) override;
    void RenderFrame() override;
    void ClearFrame() override;
    void ToggleDebug() override;
};
#endif
