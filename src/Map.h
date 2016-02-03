#ifndef MAP_H
#define	MAP_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 

#include <Core/Resources.h>
#include <Render/Surface.h>
#include <Render/Sprite.h>

enum tile{
    TILE_GRASS = 0,
    TILE_WATER,
    TILE_OBJECT
};

class Map {
public:
    Map();
    Map(const Map& orig);
    virtual ~Map();
    
    bool Load(std::string path);
    void Draw();
    void Free();
    
    bool CanMove(const Vec2& pos);
    
    bool SetTile(const Vec2& pos, tile mtile);
    tile GetTile(const Vec2& pos);
    
private:
    tile** _Tile_map;
    size_t _max_grid_x;
    size_t _max_grid_y;
    size_t _tile_grid_size;
    
    Sprite _sprite_grass;
    Sprite _sprite_water;
};

#endif	/* MAP_H */

