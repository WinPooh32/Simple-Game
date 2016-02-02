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

Object* root;
Player* player;

void Engine::OnInit() {
    Resources::SetDefaultFont("PressStart2P.ttf");
    Window::SetMode(800, 600, false, "Simple-Game v0.1");
    
    Engine::AddLayer();
    root = Engine::GetRootAtLayer(0);
    
    player = new Player;
    root->Connect(player);
}

void Engine::OnEvent(SDL_Event *event, const Uint8 *keyboardState) {

}

void Engine::OnUpdate() {

}

void Engine::OnRender() {

}

void Engine::OnCleanUp() {

}

#undef main
int main() {
    Engine engine;
    engine.Start();
    return 0;
}

