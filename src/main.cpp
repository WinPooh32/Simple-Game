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

Map* map;

Object* layer_ground;
Object* layer_objects;
Object* layer_player;
Object* layer_nature; //mountains, trees

Player* player;

int edit_tile = TILE_GRASS;

Text* label_current_tile;

bool edit_mode = false;

Timer keys_timer;

float mgrid = 1.0f / 32.0f;

void SetCurrentBrushTile(int tile){
    edit_tile = tile;

    switch (edit_tile) {
        case TILE_GRASS:
            label_current_tile->SetText("GRASS");
            break;
        case TILE_PLAIN_GRASS:
            label_current_tile->SetText("PLAIN_GRASS");
            break;
        case TILE_TREE:
            label_current_tile->SetText("TREE");
            break;
        case TILE_TREE_SPRUCE:
            label_current_tile->SetText("TREE SPRUCE");
            break;
        case TILE_WATER:
            label_current_tile->SetText("WATER");
            break;
        case TILE_MOUNTAIN:
            label_current_tile->SetText("MOUNTAIN");
            break;
        case TILE_BUILDING:
            label_current_tile->SetText("BUILDING");
            break;
        default:
            label_current_tile->SetText("UNKNOWN");
    }
}

void ProcessEditModeCamera(){
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

    Vec2 cam_offset(Window::GetCamera()->X(), Window::GetCamera()->Y());

    if (Mouse::Pressed(MOUSE_LEFT)) {
        map->SetTile((Mouse::GetPos() + cam_offset) * mgrid, static_cast<tile> (edit_tile));
    }
}

void ProcessEditMode(SDL_Event *event){
    //Switch brush by mouse wheel
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

        SetCurrentBrushTile(edit_tile);
    }
    //Set brush by keyboard's number
    else if(event->type == SDL_KEYDOWN
            && event->key.keysym.scancode >= SDL_SCANCODE_1
            && event->key.keysym.scancode <= SDL_SCANCODE_0){

        int number = (event->key.keysym.scancode - SDL_SCANCODE_1) % 10;

        SetCurrentBrushTile(number);
    }
}

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

    label_current_tile = new Text();
    label_current_tile->Init(10, 10, "GRASS", Resources::GetDefaultFont(), 14);
}

void Engine::OnEvent(SDL_Event *event, const Uint8 *keyboardState) {
    if(event->type == SDL_KEYDOWN){
        if (Keyboard::isKeyDown(KEY_E)){
            edit_mode = !edit_mode;
            player->_freecam = edit_mode;
        }

        if (Keyboard::isKeyDown(KEY_ESCAPE)){
            Engine::Stop();
        }
    }

    if (Mouse::InWindow() && edit_mode) {
        ProcessEditMode(event);
    }

}

void Engine::OnUpdate() {
    if (edit_mode) {
        ProcessEditModeCamera();
    }
}

void Engine::OnRender() {
    map->Draw();

    if (edit_mode) {
        map->DrawDebugTileRect((Mouse::GetPos() + Window::GetCamera()->GetPos()) * mgrid, static_cast<tile> (edit_tile));

        label_current_tile->Draw();
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

