#ifndef RENDERER_H
#define RENDERER_H

#include <cstring>
#include <stdio.h>

class Renderer
{
public:
    virtual void Reveal() = 0;
    virtual void Discover(int index) = 0;
    virtual void RenderFrame() = 0;
    virtual void ClearFrame() = 0;
    virtual void ToggleDebug() = 0;
};

#endif