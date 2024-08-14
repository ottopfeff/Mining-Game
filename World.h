#ifndef WORLD_H
#define WORLD_H

#include <cstdlib>
#include <vector>
#include <cstring>

typedef enum tile_t
{
    AIR,
    STONE,
    GOLD,
    EXPLOSIVE,
} tile;

class World
{
private:
    void sprinkle(int count, tile tile, bool overwrite, int indexes[]);

    void encapsulate(int count, tile tile, int index);

public:
    tile **tiles;
    int Width;
    int Height;

    World();

    World(int width, int height, int nugget_count, int stone_thickness, int explosive_count);

    ~World();

    void ChangeTile(int x, int y, tile tile);

    int DestroyTile(int x, int y);

    bool IsInBounds(int x, int y);
};
#endif