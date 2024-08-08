#include "ConsoleRenderer.h"

ConsoleRenderer::ConsoleRenderer() : world(), player(), buffer(NULL), debug(false) {}

ConsoleRenderer::ConsoleRenderer(World *world, Player *player, int width, int height)
{
    this->window_width = width;
    this->window_height = height;
    this->debug = true;
    this->world = world;
    this->player = player;
    int len = world->Width * world->Height;
    this->buffer = new char[len + world->Height + 1];
    this->discovered = new bool[len];
    memset(this->buffer, 0, len);
    memset(this->discovered, false, len);
}

ConsoleRenderer::~ConsoleRenderer()
{
    delete[] buffer;
    delete[] discovered;
}

void ConsoleRenderer::Reveal()
{
    memset(this->discovered, true, world->Width * world->Height);
}

void ConsoleRenderer::Discover(int index) // flawed
{
    int width = world->Width, height = world->Height;
    if (index < 0 || index >= width * height)
        return;
    this->discovered[index] = true;

    int adjacents[8];
    adjacents[0] = index - 1;
    adjacents[1] = index + 1;
    adjacents[2] = index - width;
    adjacents[3] = index + width;
    adjacents[4] = index - width - 1;
    adjacents[5] = index - width + 1;
    adjacents[6] = index + width - 1;
    adjacents[7] = index + width + 1;

    for (int i = 0; i < 4; i++)
    {
        int x = adjacents[i] % world->Width, y = adjacents[i] / world->Width;
        if (x > 0 && y > 0 && x < width && y < height && !discovered[adjacents[i]])
        {

            discovered[adjacents[i]] = true;
            if (world->tiles[x][y] == AIR)
                Discover(adjacents[i]);
        }
    }
}

void ConsoleRenderer::RenderFrame()
{

    int player_buffer_pos = (this->player->x + 1) + (this->player->y * (world->Width + 1));

    int j = 0; // iterator through world array

    int len = world->Width * world->Height + world->Height;
    char texture;
    for (int i = 0; i < len; i++)
    {
        int x = j % world->Width;
        int y = j / world->Width;
        if (i % (world->Width + 1) == 0)
            this->buffer[i] = '\n';
        else
        {
            if (!discovered[j])
                texture = '#';
            else
            {
                switch (world->tiles[x][y])
                {
                case AIR:
                    texture = '`';
                    break;
                case STONE:
                    texture = 'S';
                    break;
                case EXPLOSIVE:
                    texture = 'X';
                    break;
                case GOLD:
                    texture = 'G';
                    break;
                }
            }
            this->buffer[i] = texture;
            j++;
        }
    }

    switch (this->player->dir) // draw player
    {
    case LEFT:
        this->buffer[player_buffer_pos] = '<';
        break;
    case RIGHT:
        this->buffer[player_buffer_pos] = '>';
        break;
    case UP:
        this->buffer[player_buffer_pos] = '^';
        break;
    case DOWN:
        this->buffer[player_buffer_pos] = 'v';
        break;
    }

    fwrite(this->buffer, sizeof(char), len, stdout);
}

void ConsoleRenderer::ClearFrame()
{
    system("cls");
}

void ConsoleRenderer::ToggleDebug()
{
    debug = !debug;
}
