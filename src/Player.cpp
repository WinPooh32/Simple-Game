#include "Player.h"
#include "Map.h"

Vec2 sprite_size(16, 16);

Player::Player(Map* map) {
    _map = map;

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

    //Setup audio
    _birds = Resources::GetAudio("birds.ogg", AUDIO_MUSIC);
    _waves = Resources::GetAudio("waves.ogg", AUDIO_MUSIC);
}

Player::Player(const Player& orig) {
}

Player::~Player() {

}

int STEP = 32;
Vec2 right(STEP, 0);
Vec2 left(-STEP, 0);
Vec2 up(0, -STEP);
Vec2 down(0, STEP);

float grid = 1.0f / 32;

bool water = false;

void Player::Turn(turn_side side) {
    switch (side) {
        case SIDE_RIGHT:
            _sprite.SetFlip(SDL_FLIP_HORIZONTAL);
            if (_side != side) _sprite.SetAnimation(_anim_run_left);

            if (_map->CanMove((GetGlobalPos() + right) * grid))
                Move(right);
            break;

        case SIDE_LEFT:
            _sprite.SetFlip(SDL_FLIP_NONE);
            if (_side != side) _sprite.SetAnimation(_anim_run_left);

            if (_map->CanMove((GetGlobalPos() + left) * grid))
                Move(left);
            break;

        case SIDE_UP:
            if (_side != side) _sprite.SetAnimation(_anim_run_up);

            if (_map->CanMove((GetGlobalPos() + up) * grid))
                Move(up);
            break;

        case SIDE_DOWN:
            if (_side != side) _sprite.SetAnimation(_anim_run_down);

            if (_map->CanMove((GetGlobalPos() + down) * grid))
                Move(down);
            break;

        case SIDE_NONE:
            break;
    }

    if (side != SIDE_NONE) {
        _side = side;
        _sprite.SetAnimationRate(300);

        if (_map->GetTile((GetGlobalPos() + left * 3) * grid) == TILE_WATER ||
                _map->GetTile((GetGlobalPos() + right * 3) * grid) == TILE_WATER ||
                _map->GetTile((GetGlobalPos() + up * 3) * grid) == TILE_WATER ||
                _map->GetTile((GetGlobalPos() + down * 3) * grid) == TILE_WATER) {

            if (!water) {
                //Mix_FadeOutMusic(500);
                _waves->Play(-1);
                water = true;
            }
        } else {
            if (water) {
                //Mix_FadeOutMusic(500);
                _birds->Play(-1);

                water = false;
            }
        }


    }
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
    _sprite.Draw(GetGlobalPos(), GetSize()*2, Window::GetCamera());
}
