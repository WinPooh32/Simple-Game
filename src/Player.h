#ifndef PLAYER_H
#define	PLAYER_H

#include <iostream>

#include <Core/Object.h>
#include <Core/Vec2.h>
#include <Core/Timer.h>
#include <Core/Resources.h>

#include <Render/Sprite.h>
#include <Render/Animation.h>

#include <IO/Keyboard.h>

#include "globals.h"

enum turn_side{
    SIDE_NONE = 0, // staying
    SIDE_UP,
    SIDE_DOWN,
    SIDE_LEFT,
    SIDE_RIGHT
};

class Player: public Object {
public:
    Player();
    Player(const Player& orig);
    virtual ~Player();
    
    void Turn(turn_side side);
    
    void OnUpdate();
    void OnRender();
    
private:
    Sprite _sprite;
    
    Animation _anim_stay;
    Animation _anim_run_up;
    Animation _anim_run_down;
    Animation _anim_run_left;
    
    turn_side _side;
    
    Timer _input_timer;
};

#endif	/* PLAYER_H */

