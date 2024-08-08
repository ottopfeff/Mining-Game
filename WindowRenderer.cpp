#include "WindowRenderer.h"

WindowRenderer::WindowRenderer() {}
WindowRenderer::WindowRenderer(World *world, Player *player, int width, int height)
{

    _World = world;
    _Player = player;
    Width = width;
    Height = height;
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
    SDL_Quit();
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
                SDL_SetRenderDrawColor(Renderer, 255, 255, 100, 255);
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
        //rect.x = xRem;
    }

    rect.x = xRem + (_Player->x * tileLength);
    rect.y = _Player->y * tileLength;

    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(Renderer, &rect);

    SDL_RenderPresent(Renderer);

    if (this->debug)
    {
        printf("\n(%d,%d)  ", this->_Player->x, this->_Player->y);
        switch (this->_Player->dir)
        {
        case LEFT:
            printf("LEFT  ");
            break;
        case RIGHT:
            printf("RIGHT  ");
            break;
        case DOWN:
            printf("DOWN  ");
            break;
        case UP:
            printf("UP  ");
            break;
        }
        printf("SCORE: %d", this->_Player->score);
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