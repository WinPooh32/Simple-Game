#include "Player.h"

Vec2 sprite_size(16, 16);

Player::Player() {
    SetSize(sprite_size);

    _side = SIDE_LEFT;

    //Staying anim depends on looking side
    _anim_stay.SetBeginFrame(0);
    _anim_stay.SetMaxFrame(0);
    _anim_stay.SetCurrentFrame(0);

    //Running up side
    _anim_run_up.SetBeginFrame(4);
    _anim_run_up.SetMaxFrame(5);
    _anim_run_up.SetCurrentFrame(4);

    //Running down side
    _anim_run_down.SetBeginFrame(0);
    _anim_run_down.SetMaxFrame(1);
    _anim_run_down.SetCurrentFrame(0);

    //Running left side
    _anim_run_left.SetBeginFrame(2);
    _anim_run_left.SetMaxFrame(3);
    _anim_run_left.SetCurrentFrame(2);
    

    //Setup sprite
    _sprite.SetTexture(Resources::GetTexture("blowhard.png"));
    _sprite.SetAnimation(_anim_run_left);
    _sprite.SetFrameSize(sprite_size);
    _sprite.SetAnimationRate(300);
}

Player::Player(const Player& orig) {
}

Player::~Player() {

}

void Player::Turn(turn_side side) {
    switch (side) {
        case SIDE_RIGHT:
            _sprite.SetFlip(SDL_FLIP_HORIZONTAL);
            if(_side != side) _sprite.SetAnimation(_anim_run_left);
            
            Move(Vec2(16, 0));
            break;

        case SIDE_LEFT:
            _sprite.SetFlip(SDL_FLIP_NONE);
            if(_side != side) _sprite.SetAnimation(_anim_run_left);
            
            Move(Vec2(-16, 0));
            break;

        case SIDE_UP:
            if(_side != side) _sprite.SetAnimation(_anim_run_up);
            
            Move(Vec2(0, -16));
            break;

        case SIDE_DOWN:
            if(_side != side) _sprite.SetAnimation(_anim_run_down);
            
            Move(Vec2(0, 16));
            break;
            
        case SIDE_NONE:
            break;
    }

    if (side != SIDE_NONE) {
        _side = side;
    }
     _sprite.SetAnimationRate(300);
}

void Player::OnUpdate() {
    _input_timer.Start();
    if (_input_timer.GetTime() > 100) {
        if (Keyboard::isKeyDown(KEY_RIGHT)) {
            Turn(SIDE_RIGHT);
        } else if (Keyboard::isKeyDown(KEY_LEFT)) {
            Turn(SIDE_LEFT);
        } else if (Keyboard::isKeyDown(KEY_UP)) {
            Turn(SIDE_UP);
        } else if (Keyboard::isKeyDown(KEY_DOWN)) {
            Turn(SIDE_DOWN);
        } else {
            Turn(SIDE_NONE);
        }
        _input_timer.Stop();
    }
}

void Player::OnRender() {
    _sprite.Draw(GetGlobalPos(), GetSize()*4, Window::GetCamera());
}
