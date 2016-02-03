#include "Map.h"

Map::Map() {
    _max_grid_x = 0;
    _max_grid_y = 0;
    _tile_grid_size = 0;
    _Tile_map = nullptr;
    
    Vec2 frame(16, 16);
    
    //setup grass
    _sprite_grass.SetTexture(Resources::GetTexture("blowhard.png"));
    _sprite_grass.SetFrameSize(frame);
    _sprite_grass.SetFrame(10);
    
    //setup water
    _sprite_water.SetTexture(Resources::GetTexture("blowhard.png"));
    _sprite_water.SetFrameSize(frame);
    _sprite_water.SetFrame(16);
    
    //setup mountain
    _sprite_mountain.SetTexture(Resources::GetTexture("blowhard.png"));
    _sprite_mountain.SetFrameSize(frame);
    _sprite_mountain.SetFrame(14);
    
    //setup tree
    _sprite_tree.SetTexture(Resources::GetTexture("blowhard.png"));
    _sprite_tree.SetFrameSize(frame);
    _sprite_tree.SetFrame(12);
}

Map::Map(const Map& orig) {

}

Map::~Map() {

}

bool Map::Load(std::string path) {
    std::fstream file_map(path);

    size_t width;
    size_t height;

    file_map >> width;
    file_map >> height;

    if (width != 0 && height != 0) {
        _max_grid_x = width;
        _max_grid_y = height;
        
        std::cout << "Map size: w "<< width << ", h "<< height << std::endl; 
    } else {
        std::cerr << "Invalid sizes: " << width << ", " << height << std::endl;
        return false;
    }

    //Init map array
    _Tile_map = new tile*[_max_grid_y];
    for (size_t i = 0; i < _max_grid_y; ++i) {
        _Tile_map[i] = new tile[_max_grid_x];
    }

    std::string line;
    size_t words_count;
    size_t lines_count = 0;
    
    while (getline(file_map, line)) {

        //skip null line
        if(line.empty()){
            continue;
        }
        
        words_count = 0;

        std::stringstream line_stream(line);
        size_t ntile;
        
        while (line_stream >> ntile) {
            if(ntile > TILE_NONE){
                std::cerr << "Wrong tile: " << ntile << std::endl;
                return false;
            }
            
            _Tile_map[lines_count][words_count] = static_cast<tile> (ntile);
            ++words_count;
        }

        if (words_count != _max_grid_x) {
            std::cerr << "Wrong map size: " << words_count << ", expected " << _max_grid_x << std::endl;
            return false;
        }

        ++lines_count;
    }
    
    return true;
}

void Map::Draw(){
    Sprite* draw_sprite;
    
    for(size_t i = 0; i < _max_grid_y; ++i){
        for(size_t j = 0; j < _max_grid_x; ++j){
            switch(_Tile_map[i][j]){
                case TILE_OBJECT:
                case TILE_GRASS:
                    draw_sprite = &_sprite_grass;
                    break;
                    
                case TILE_MOUNTAIN:
                    draw_sprite = &_sprite_mountain;
                    break;
                    
                case TILE_TREE:
                    draw_sprite = &_sprite_tree;
                    break;
                    
                case TILE_WATER:
                    draw_sprite = &_sprite_water;
                    break;
            }
            
            draw_sprite->Draw(Vec2(j, i)*32, Vec2(32, 32), Window::GetCamera());
        }
    }
}

void Map::Free() {
    for (size_t i = 0; i < _max_grid_x; ++i) {
        delete[](_Tile_map[i]);
    }
    delete[](_Tile_map);

    _max_grid_x = 0;
    _max_grid_y = 0;
    _tile_grid_size = 0;
    _Tile_map = nullptr;
}

bool Map::CanMove(const Vec2& pos){
    if(pos.x < 0 || pos.y < 0 || pos.x >= _max_grid_x || pos.y >= _max_grid_y){
        return false;
    }
    
    tile mtile = _Tile_map[static_cast<int>(pos.y)][static_cast<int>(pos.x)];
    if(mtile == TILE_WATER || mtile == TILE_MOUNTAIN || mtile == TILE_BUILDING){
        return false;
    }
    
    return true;
}

bool Map::SetTile(const Vec2& pos, tile mtile){
    if(pos.x < 0 || pos.y < 0 || pos.x >= _max_grid_x || pos.y >= _max_grid_y){
        return false;
    }
    
    _Tile_map[static_cast<int>(pos.y)][static_cast<int>(pos.x)] = mtile;
    
    return true;
}

tile Map::GetTile(const Vec2& pos){
    if(pos.x < 0 || pos.y < 0 || pos.x >= _max_grid_x || pos.y >= _max_grid_y){
        return TILE_WATER;
    }
  
    return _Tile_map[static_cast<int>(pos.y)][static_cast<int>(pos.x)];
}