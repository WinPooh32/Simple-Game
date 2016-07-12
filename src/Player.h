#ifndef PLAYER_H
#define	PLAYER_H

#include <iostream>

#include <Core/Object.h>
#include <Core/Vec2.h>
#include <Core/Timer.h>
#include <Core/Resources.h>
#include <Core/Audio.h>

#include <Render/Sprite.h>
#include <Render/Animation.h>

#include <IO/Keyboard.h>
#include <IO/Mouse.h>

#include "globals.h"
#include "Map.h"
#include "SoundManager.h"

enum turn_side {
    SIDE_NONE = 0, // staying
    SIDE_UP,
    SIDE_DOWN,
    SIDE_LEFT,
    SIDE_RIGHT
};

class Player : public Object {
public:
    bool _freecam = false;

    Player(Map* map);
    ~Player();

    void Turn(turn_side side);

    void OnUpdate();
    void OnRender();

private:
    static const int GRASS_SPEED = 100;
    static const int TREE_SPEED = 500;
    
    unsigned int _speed; //keyboard delay, lower is faster
    bool _water_sounds;
    
    Map* _map;
    SoundManager* _sound_mgr;

    Sprite _sprite;

    Animation _anim_stay;
    Animation _anim_run_up;
    Animation _anim_run_down;
    Animation _anim_run_left;

    turn_side _side;

    Timer _input_timer;
    Timer _music_change_timer;

    Audio* _waves;
    Audio* _birds;

    void MoveIt(const Vec2& offset);
};

#endif	/* PLAYER_H */

