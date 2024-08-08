#include <stdlib.h>
#include <array>
#include "World.h"

using namespace std;

World::World()
{
    Width = 20;
    Height = 20;
    this->tiles = new tile *[Width];

    for (int i = 0; i < Width; i++)
    {
        this->tiles[i] = new tile[Height];
        memset(tiles[i], AIR, Height);
    }
};

World::World(int width, int height, int nugget_count, int stone_thickness, int explosive_count)
{
    this->Width = width;
    this->Height = height;
    this->tiles = new tile *[Width];

    for (int i = 0; i < Width; i++)
    {
        this->tiles[i] = new tile[Height];
        for (int j = 0; j < Height; j++)
            this->tiles[i][j] = AIR;
    }

    int *nugget_arr = new int[nugget_count];
    sprinkle(nugget_count, GOLD, false, nugget_arr);
    sprinkle(nugget_count, GOLD, false, NULL);
    for (int i = 0; i < nugget_count; i++)
        this->encapsulate(stone_thickness, STONE, nugget_arr[i]);
    delete[] nugget_arr;

    sprinkle(explosive_count, EXPLOSIVE, false, NULL);
}
World::~World()
{
    for (int i = 0; i < Width; i++)
    {
        delete[] tiles[i];
    }
    delete[] tiles;
}

void World::ChangeTile(int x, int y, tile tile)
{
    if (x > 0 && y > 0 && x < Width && y < Height)
        this->tiles[x][y] = tile;
}

void World::sprinkle(int count, tile tile, bool overwrite, int indexes[])
{
    int i = 0;
    while (i < count)
    {
        int randX = rand() % Width;
        int randY = rand() % Height;
        if (this->tiles[randX][randY] == AIR)
        {
            this->tiles[randX][randY] = tile;
            if (indexes)
                indexes[i] = randX + randY * Width;
            i++;
        }
        else
            continue;
    }
}

void World::encapsulate(int count, tile tile, int index)
{
    if (count == 0)
        return;

    int adjacents[4];
    adjacents[0] = index - 1;
    adjacents[1] = index + 1;
    adjacents[2] = index - Width;
    adjacents[3] = index + Width;

    for (int i = 0; i < 4; i++)
    {
        int x = adjacents[i] % Width, y = adjacents[i] / Width;
        if (x < Width && y < Height && x >= 0 && y >= 0 && this->tiles[x][y] == AIR)
        {
            this->tiles[x][y] = tile;
        }
    }

    this->encapsulate(count - 1, tile, adjacents[rand() % 4]);
}

int World::DestroyTile(int x, int y)
{
    if (x < 0 || y < 0 || x >= Width || y >= Height)
        return 0;
    int score = 0;
    int index = x + y * Width;
    switch (tiles[x][y])
    {
    case AIR:
        break;
    case STONE:
        this->tiles[x][y] = AIR;
        break;
    case EXPLOSIVE:
        int adjacents[8];
        adjacents[0] = index - 1;
        adjacents[1] = index + 1;
        adjacents[2] = index - Width;
        adjacents[3] = index + Width;
        adjacents[4] = index - Width - 1;
        adjacents[5] = index - Width + 1;
        adjacents[6] = index + Width - 1;
        adjacents[7] = index + Width + 1;

        tiles[x][y] = AIR;
        for (int i = 0; i < 8; i++)
        {
            if (adjacents[i] < Width * Height && adjacents[i] > 0)
                score += DestroyTile(adjacents[i] % Width, adjacents[i] / Width);
        }
        break;
    case GOLD:
        score = 1;
        tiles[x][y] = AIR;
        break;
    }
    return score;
}