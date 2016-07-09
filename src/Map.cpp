#include "Map.h"

constexpr float Map::GRID_SCALE;
constexpr int Map::SCALED_TILE_SIZE;

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

    //setup plain grass
    _sprite_plain_grass.SetTexture(Resources::GetTexture("blowhard.png"));
    _sprite_plain_grass.SetFrameSize(frame);
    _sprite_plain_grass.SetFrame(22);

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

    _sprite_tree_spruce.SetTexture(Resources::GetTexture("blowhard.png"));
    _sprite_tree_spruce.SetFrameSize(frame);
    _sprite_tree_spruce.SetFrame(32);

    _nosprite.SetFrameSize(frame);
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

        std::cout << "Map size: w " << width << ", h " << height << std::endl;
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

    while (getline(file_map, line) && lines_count < _max_grid_y) {

        //skip null line
        if (line.empty()) {
            continue;
        }

        words_count = 0;

        std::stringstream line_stream(line);
        size_t ntile;

        while (line_stream >> ntile && words_count < _max_grid_x) {
            if (ntile > TILE_NONE) {
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

    file_map.close();

    return true;
}

bool Map::SaveMap(std::string path) {
    std::ofstream file_map(path, std::ios::trunc);

    file_map << _max_grid_x << " " << _max_grid_y << std::endl;

    for (size_t i = 0; i < _max_grid_y; ++i) {
        for (size_t j = 0; j < _max_grid_x; ++j) {
            file_map << _Tile_map[i][j] << " ";
        }
        file_map << std::endl;
    }

    file_map.close();

    return true;
}

void Map::DrawTile(const Vec2& pos_local, tile mtile) {

    tile left, right, up, down;

    Sprite* draw_sprite;
    switch (mtile) {
        case TILE_OBJECT:
        case TILE_PLAIN_GRASS:
            draw_sprite = &_sprite_plain_grass;
            break;

        case TILE_GRASS:
            draw_sprite = &_sprite_grass;
            break;

        case TILE_MOUNTAIN:
            draw_sprite = &_sprite_mountain;
            break;

        case TILE_TREE:
            draw_sprite = &_sprite_tree;
            break;

        case TILE_TREE_SPRUCE:
            draw_sprite = &_sprite_tree_spruce;
            break;

        case TILE_WATER:
            draw_sprite = &_sprite_water;

            _sprite_water.SetFlip(SDL_FLIP_NONE);

            left = GetTile(pos_local + Vec2::Left);
            right = GetTile(pos_local + Vec2::Right);
            up = GetTile(pos_local + Vec2::Up);
            down = GetTile(pos_local + Vec2::Down);

            if (left != TILE_WATER && right != TILE_WATER && up != TILE_WATER && down != TILE_WATER) {
                _sprite_water.SetFrame(20);
            } else {


                //ALL sides is WATER
                if (right == TILE_WATER && left == TILE_WATER && down == TILE_WATER && up == TILE_WATER) {
                    _sprite_water.SetFrame(16);
                }                    //RIGHT is WATER
                else if (right == TILE_WATER) {
                    if (up == TILE_WATER && down == TILE_WATER) {
                        _sprite_water.SetFrame(25);
                    } else if (left == TILE_WATER && down == TILE_WATER) {
                        _sprite_water.SetFrame(24);
                    } else if (left == TILE_WATER && up == TILE_WATER) {
                        _sprite_water.SetFrame(24);
                        _sprite_water.SetFlip(SDL_FLIP_VERTICAL);
                    } else if (up != TILE_WATER && down == TILE_WATER) {
                        _sprite_water.SetFrame(28);
                    } else if (up == TILE_WATER && down != TILE_WATER) {
                        _sprite_water.SetFrame(28);
                        _sprite_water.SetFlip(SDL_FLIP_VERTICAL);
                    } else if (left == TILE_WATER) {
                        _sprite_water.SetFrame(18);
                    } else {
                        _sprite_water.SetFrame(27);
                        _sprite_water.SetFlip(SDL_FLIP_HORIZONTAL);
                    }
                }                    //LEFT is WATER
                else if (left == TILE_WATER) {
                    if (up == TILE_WATER && down == TILE_WATER) {
                        _sprite_water.SetFrame(25);
                        _sprite_water.SetFlip(SDL_FLIP_HORIZONTAL);
                    } else if (up != TILE_WATER && down == TILE_WATER) {
                        _sprite_water.SetFrame(28);
                        _sprite_water.SetFlip(SDL_FLIP_HORIZONTAL);
                    } else if (up == TILE_WATER && down != TILE_WATER) {
                        _sprite_water.SetFrame(29);
                    } else {
                        _sprite_water.SetFrame(27);
                    }
                }                    //UP is WATER
                else if (up == TILE_WATER) {
                    if (down == TILE_WATER) {
                        _sprite_water.SetFrame(19);
                    } else {
                        _sprite_water.SetFrame(26);
                    }
                }                    //DOWN is WATER
                else if (down == TILE_WATER) {
                    if (up == TILE_WATER) {
                        _sprite_water.SetFrame(19);
                    } else if (right == TILE_WATER) {
                        _sprite_water.SetFrame(99);
                    } else {
                        _sprite_water.SetFrame(26);
                        _sprite_water.SetFlip(SDL_FLIP_VERTICAL);
                    }
                }


                //                if (right == TILE_WATER && up == TILE_WATER && down == TILE_WATER && up == TILE_WATER) {
                //                    _sprite_water.SetFrame(16);
                //                }
                //                else if (up == TILE_WATER && down == TILE_WATER) {
                //                    _sprite_water.SetFrame(19);
                //                } 
                //                else if (right == TILE_WATER && up == TILE_WATER && down == TILE_WATER) {
                //                    _sprite_water.SetFrame(25);
                //                }
                //                else if (left == TILE_WATER && up == TILE_WATER && down == TILE_WATER) {
                //                    _sprite_water.SetFrame(25);
                //                    _sprite_water.SetFlip(SDL_FLIP_HORIZONTAL);
                //                }
                //                else {
                //
                //                }

            }

            break;

        default:
            draw_sprite = &_nosprite;
    }

    draw_sprite->Draw(pos_local * 32, Vec2(32, 32), Window::GetCamera());
}

void Map::Draw() {
    Vec2 local_pos;
    for (size_t i = 0; i < _max_grid_y; ++i) {
        for (size_t j = 0; j < _max_grid_x; ++j) {
            local_pos.x = j;
            local_pos.y = i;
            DrawTile(local_pos, _Tile_map[i][j]);
        }
    }
}

void Map::DrawDebugTileRect(const Vec2& pos, tile mtile) {
    if (pos.x < 0 || pos.y < 0 || pos.x >= _max_grid_x || pos.y >= _max_grid_y) {
        return;
    }

    SDL_Rect tile_border_pos = {(int)pos.x*32 - Window::GetCamera()->X() - 1,
                            (int)pos.y*32 - Window::GetCamera()->Y() - 1,
                            34, 34};
    Surface::DrawRect(&tile_border_pos, COLOR_YELLOW);

    DrawTile(Vec2((int) pos.x, (int) pos.y), mtile);
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

bool Map::CanMove(const Vec2& pos) {
    if (pos.x < 0 || pos.y < 0 || pos.x >= _max_grid_x || pos.y >= _max_grid_y) {
        return false;
    }

    tile mtile = _Tile_map[static_cast<int> (pos.y)][static_cast<int> (pos.x)];
    if (mtile == TILE_WATER || mtile == TILE_MOUNTAIN || mtile == TILE_BUILDING) {
        return false;
    }

    return true;
}

bool Map::CanMove(tile mtile) {
    switch (mtile) {
        case TILE_WATER:
        case TILE_MOUNTAIN:
        case TILE_BUILDING:
            return false;
        default:
            return true;
    }
}

bool Map::SetTile(const Vec2& pos, tile mtile) {
    if (pos.x < 0 || pos.y < 0 || pos.x >= _max_grid_x || pos.y >= _max_grid_y) {
        return false;
    }

    _Tile_map[static_cast<int> (pos.y)][static_cast<int> (pos.x)] = mtile;

    return true;
}

tile Map::GetTile(const Vec2& pos) {
    if (pos.x < 0 || pos.y < 0 || pos.x >= _max_grid_x || pos.y >= _max_grid_y) {
        return TILE_WATER;
    }

    return _Tile_map[static_cast<int> (pos.y)][static_cast<int> (pos.x)];
}

tile Map::GetTileNear(const Vec2& pos, const Vec2& side_offset){
    Vec2 next(pos.x + side_offset.x, pos.y + side_offset.y);
    return GetTile(next);
}
