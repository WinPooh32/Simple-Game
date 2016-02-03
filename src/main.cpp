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

Map* map;

Object* layer_ground;
Object* layer_objects;
Object* layer_player;
Object* layer_nature; //mountains, trees

Player* player;

void Engine::OnInit() {
    Resources::SetDefaultFont("PressStart2P.ttf");
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
}

void Engine::OnEvent(SDL_Event *event, const Uint8 *keyboardState) {

}

float mgrid = 1.0f/32.0f;

void Engine::OnUpdate() {
    if (Mouse::InWindow()) {
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
        
        if(Mouse::Pressed(MOUSE_LEFT)){
            map->SetTile((Mouse::GetPos()+camoffset) * mgrid, TILE_GRASS);
        }
        else if(Mouse::Pressed(MOUSE_RIGHT)){
            map->SetTile((Mouse::GetPos()+camoffset) * mgrid, TILE_WATER);
        }
    }
}

void Engine::OnRender() {
    map->Draw();
}

void Engine::OnCleanUp() {
    Resources::UnloadAll();
    delete(map);
}

#undef main

int main() {
    Engine engine;
    engine.Start();
    return 0;
}

