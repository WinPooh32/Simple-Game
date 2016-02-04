/*!
 boldtemple Gaming ©, http://gaming.boldtemple.net
 OpenSource Project - Check Development, License & Readme Sections.

 BGE - boldtemple Cross Platform Game Engine
 /main.cpp
 !*/

#include <iostream>
#include <string>

#include <Engine.h>

#include "Player.h"
#include "Map.h"
#include "GUI/Text.h"
//#include "Tree.h"

Map* map;

Object* layer_ground;
Object* layer_objects;
Object* layer_player;
Object* layer_nature; //mountains, trees

Player* player;

int edit_tile = TILE_GRASS;

Text* current_tile;

bool edit_mode = false;

Timer keys_timer;

void Engine::OnInit() {
    Resources::SetDefaultFont("PressStart2P.ttf");
    //Window::SetMode(1920, 1080, true, "Simple-Game v0.1");
    Window::SetMode(800, 600, false, "Simple-Game v0.1");
    //SDL_SetRelativeMouseMode(SDL_bool(1));

    Engine::AddLayer();
    Engine::AddLayer();
    Engine::AddLayer();
    Engine::AddLayer();

    layer_ground = Engine::GetRootAtLayer(0);
    layer_objects = Engine::GetRootAtLayer(1);
    layer_player = Engine::GetRootAtLayer(2);
    layer_nature = Engine::GetRootAtLayer(3);

    map = new Map;
    map->Load("map.txt");

    player = new Player(map);
    layer_player->Connect(player);

    current_tile = new Text();
    current_tile->Init(10, 10, "GRASS", Resources::GetDefaultFont(), 14);
}

void Engine::OnEvent(SDL_Event *event, const Uint8 *keyboardState) {

}

float mgrid = 1.0f / 32.0f;

void Engine::OnUpdate() {

    keys_timer.Start();
    if (keys_timer.GetTime() > 100) {

        if (Keyboard::isKeyDown(KEY_E)) {
            edit_mode = !edit_mode;
            player->_freecam = edit_mode;
        }
        keys_timer.Stop();
    }

    if (Mouse::InWindow() && edit_mode) {
        int x, y;
        Mouse::GetPos(&x, &y);

        if (x < 10) {
            Window::GetCamera()->Move(Vec2(-5, 0));
        } else if (x > Window::GetWidth() - 10) {
            Window::GetCamera()->Move(Vec2(5, 0));
        }

        if (y < 10) {
            Window::GetCamera()->Move(Vec2(0, -5));
        } else if (y > Window::GetHeight() - 10) {
            Window::GetCamera()->Move(Vec2(0, 5));
        }

        Vec2 camoffset(Window::GetCamera()->X(), Window::GetCamera()->Y());
        //tile mtile = map->GetTile((Mouse::GetPos() + camoffset) * mgrid);

        if (Mouse::Pressed(MOUSE_LEFT)) {
            map->SetTile((Mouse::GetPos() + camoffset) * mgrid, static_cast<tile> (edit_tile));
        }

        if (Mouse::AnyWheeled()) {
            if (Mouse::Wheeled(MOUSE_WHEEL_UP)) {
                ++edit_tile;
            } else {
                --edit_tile;
            }

            if (edit_tile >= TILE_NONE) {
                edit_tile = TILE_GRASS;
            } else if (edit_tile < 0) {
                edit_tile = TILE_NONE - 1;
            }

            switch (edit_tile) {
                case TILE_GRASS:
                    std::cout << "GRASS" << std::endl;
                    current_tile->SetText("GRASS");
                    break;
                case TILE_PLAIN_GRASS:
                    std::cout << "PLAIN_GRASS" << std::endl;
                    current_tile->SetText("PLAIN_GRASS");
                    break;
                case TILE_TREE:
                    std::cout << "TREE" << std::endl;
                    current_tile->SetText("TREE");
                    break;
                case TILE_TREE_SPRUCE:
                    std::cout << "TREE SPRUCE" << std::endl;
                    current_tile->SetText("TREE SPRUCE");
                    break;
                case TILE_WATER:
                    std::cout << "WATER" << std::endl;
                    current_tile->SetText("WATER");
                    break;
                case TILE_MOUNTAIN:
                    std::cout << "MOUNTAIN" << std::endl;
                    current_tile->SetText("MOUNTAIN");
                    break;
                case TILE_BUILDING:
                    std::cout << "BUILDING" << std::endl;
                    current_tile->SetText("BUILDING");
                    break;
                default:
                    std::cout << "UNKNOWN" << std::endl;
                    current_tile->SetText("UNKNOWN");
            }

        }
    }

}

void Engine::OnRender() {
    map->Draw();

    if (edit_mode) {
        Vec2 camoffset(Window::GetCamera()->X(), Window::GetCamera()->Y());
        map->DrawDebugTileRect((Mouse::GetPos() + camoffset) * mgrid, static_cast<tile> (edit_tile));

        current_tile->Draw();
    }
}

void Engine::OnCleanUp() {
    map->SaveMap("./Maps/saved.txt");

    Resources::UnloadAll();
    delete(map);
}

#undef main

int main() {
    Engine engine;
    engine.Start();
    return 0;
}

