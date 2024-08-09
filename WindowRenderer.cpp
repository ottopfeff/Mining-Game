#include "WindowRenderer.h"

WindowRenderer::WindowRenderer() {}
WindowRenderer::WindowRenderer(World *world, Player *player, int width, int height)
{

    _World = world;
    _Player = player;
    Width = width;
    Height = height;
    MouseX = 0;
    MouseY = 0;
    int size = _World->Width * _World->Height;
    Discovered = new bool[size];
    memset(Discovered, false, size);
    debug = true;
    Init_Window("Mining Game");
    Init_Renderer();
}
WindowRenderer::~WindowRenderer()
{
    if (Window)
        SDL_DestroyWindow(Window);
    if (Renderer)
        SDL_DestroyRenderer(Renderer);
    delete[] Discovered;
}
void WindowRenderer::Reveal() { memset(Discovered, true, _World->Width * _World->Height); }
void WindowRenderer::Discover(int index) {}
void WindowRenderer::RenderFrame()
{
    int tileLength = Height / _World->Height;
    int xRem = (Width - Height) / 2;

    SDL_Rect rect;
    rect.x = xRem;
    rect.y = 0;
    rect.w = tileLength;
    rect.h = tileLength;

    for (int i = 0; i < _World->Width; i++)
    {
        for (int j = 0; j < _World->Height; j++)
        {
            switch (_World->tiles[i][j])
            {
            case AIR:
                SDL_SetRenderDrawColor(Renderer, 0, 255, 0, 255);
                break;
            case STONE:
                SDL_SetRenderDrawColor(Renderer, 75, 75, 75, 255);
                break;
            case EXPLOSIVE:
                SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
                break;
            case GOLD:
                SDL_SetRenderDrawColor(Renderer, 255, 255, 0, 255);
                break;
            default:
                SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
            }
            SDL_RenderFillRect(Renderer, &rect);
            rect.y += tileLength;
        }
        rect.x += tileLength;
        rect.y = 0;
    }

    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(Renderer, &rect);
    DrawPlayer(tileLength, xRem);
    DrawSelectedTile(tileLength, xRem);

    SDL_RenderPresent(Renderer);

    if (this->debug)
    {
        printf("\n(%f,%f) GAME ", this->_Player->x, this->_Player->y);
        printf("\n(%d,%d) MOUSE ", MouseX, MouseY);
        printf("\nSCORE: %d", this->_Player->score);
        printf("\n");
    }
}
void WindowRenderer::ClearFrame()
{
    SDL_RenderClear(Renderer);
    system("cls");
}
void WindowRenderer::ToggleDebug() { debug = !debug; }
void WindowRenderer::Init_Window(const char *title)
{
    Window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, SDL_WINDOW_ALLOW_HIGHDPI);
    if (!Window)
    {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        exit(1);
    }
}

void WindowRenderer::Init_Renderer()
{
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_SOFTWARE);
    if (!Renderer)
    {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        exit(1);
    }
}

void WindowRenderer::DrawPlayer(int tileLength, int xRem)
{
    SDL_Vertex vertices[3];
    float x = _Player->x, y = _Player->y;

    vertices[0].position = {x * tileLength + xRem, y * tileLength};
    vertices[0].color = {0, 0, 0, 255};
    vertices[1].position = {x * tileLength + xRem, (y + 1.0f) * tileLength};
    vertices[1].color = {0, 0, 0, 255};
    vertices[2].position = {(x + 1.0f) * tileLength + xRem, (y + 1.0f) * tileLength};
    vertices[2].color = {0, 0, 0, 255};
    vertices[3].position = {(x + 1) * tileLength + xRem, y * tileLength};
    vertices[3].color = {0, 0, 0, 255};

    int indices[6] = {0, 1, 2, 0, 3, 2};

    SDL_RenderGeometry(Renderer, nullptr, vertices, 4, indices, 6);
}

void WindowRenderer::DrawSelectedTile(int tileLength, int xRem)
{
    if (MouseX < xRem || MouseX > MouseX + _World->Width)
        return;

    int x = (MouseX - xRem) / tileLength, y = MouseY / tileLength;
    int rendX = x * tileLength + xRem, rendY = y * tileLength;

    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(Renderer, rendX, rendY, rendX + tileLength, rendY);
    SDL_RenderDrawLine(Renderer, rendX, rendY, rendX, rendY + tileLength);
    SDL_RenderDrawLine(Renderer, rendX + tileLength, rendY, rendX + tileLength, rendY + tileLength);
    SDL_RenderDrawLine(Renderer, rendX, rendY + tileLength, rendX + tileLength, rendY + tileLength);
}