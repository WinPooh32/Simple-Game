#ifndef MAP_H
#define	MAP_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 

#include <Core/Resources.h>
#include <Render/Surface.h>
#include <Render/Sprite.h>

enum tile {
    TILE_GRASS = 0,
    TILE_WATER,
    TILE_PLAIN_GRASS,
    TILE_TREE,
    TILE_MOUNTAIN,
    TILE_TREE_SPRUCE,
    TILE_OBJECT,
    TILE_BUILDING,
    TILE_NONE // END OF TILES
};

class Map {
public:
    //static constexpr int SCALED_TILE_SIZE = 16 * 2;
    //static constexpr float GRID_SCALE = 1.0f / SCALED_TILE_SIZE;
    static int SCALED_TILE_SIZE;
    static float GRID_SCALE;

    static void Zoom(float scale);

    Map();
    Map(const Map& orig);
    virtual ~Map();

    bool Load(std::string path);
    bool SaveMap(std::string path);

    void Draw();
    void DrawDebugTileRect(const Vec2& pos, tile mtile);
    void Free();

    bool CanMove(const Vec2& pos);
    bool CanMove(tile mtile);

    bool SetTile(const Vec2& pos, tile mtile);
    inline tile GetTile(const Vec2& pos);
    tile GetTileNear(const Vec2& pos, const Vec2& side_offset);

    size_t GetW();
    size_t GetH();

private:
    tile** _Tile_map;
    size_t _max_grid_x;
    size_t _max_grid_y;
    size_t _tile_grid_size;

    Sprite _nosprite;

    Sprite _sprite_grass;
    Sprite _sprite_plain_grass;
    Sprite _sprite_water;
    Sprite _sprite_water_border;
    Sprite _sprite_mountain;
    Sprite _sprite_tree;
    Sprite _sprite_tree_spruce;

    void DrawTile(const Vec2& pos_local, tile mtile);
};

inline
tile Map::GetTile(const Vec2& pos) {
    if (pos.x < 0 || pos.y < 0 || pos.x >= _max_grid_x || pos.y >= _max_grid_y) {
        return TILE_WATER;
    }

    return _Tile_map[static_cast<int> (pos.y)][static_cast<int> (pos.x)];
}

#endif	/* MAP_H */

