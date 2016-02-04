#include "Player.h"

Player::Player(Map* map) {
    _speed = TREE_SPEED;
    _water_sounds = false;
    _map = map;
    _side = SIDE_LEFT;
    
    Vec2 sprite_size(16, 16);
    SetSize(sprite_size);

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

    _birds->Play(-1);
}

void Player::MoveIt(const Vec2& offset) {
    tile mtile = _map->GetTile(GetGlobalPos() * Map::GRID_SCALE + offset);
    if (_map->CanMove(mtile)) {
        if (mtile == TILE_TREE || mtile == TILE_TREE_SPRUCE) {
            _speed = TREE_SPEED;
        } else {
            _speed = GRASS_SPEED;
        }
        Move(offset * Map::SCALED_TILE_SIZE);
    }
}

void Player::Turn(turn_side side) {
    switch (side) {
        case SIDE_RIGHT:
            _sprite.SetFlip(SDL_FLIP_HORIZONTAL);
            if (_side != side) _sprite.SetAnimation(_anim_run_left);
            MoveIt(Vec2::Right);
            break;

        case SIDE_LEFT:
            _sprite.SetFlip(SDL_FLIP_NONE);
            if (_side != side) _sprite.SetAnimation(_anim_run_left);
            MoveIt(Vec2::Left);
            break;

        case SIDE_UP:
            if (_side != side) _sprite.SetAnimation(_anim_run_up);
            MoveIt(Vec2::Up);
            break;

        case SIDE_DOWN:
            if (_side != side) _sprite.SetAnimation(_anim_run_down);
            MoveIt(Vec2::Down);
            break;

        case SIDE_NONE:
            break;
    }

    if (side != SIDE_NONE) {
        _side = side;
        _sprite.SetAnimationRate(300);
    }

    _music_change_timer.Start();
    if (_music_change_timer.GetTime() > 3000) {
        
        Vec2 grid_pos = GetGlobalPos() * Map::GRID_SCALE;
        
        if (_map->GetTileNear(grid_pos, Vec2::Left) == TILE_WATER  ||
            _map->GetTileNear(grid_pos, Vec2::Right) == TILE_WATER ||
            _map->GetTileNear(grid_pos, Vec2::Up) == TILE_WATER    ||
            _map->GetTileNear(grid_pos, Vec2::Down) == TILE_WATER ){

            if (!_water_sounds) {
                _waves->Play(-1);
                _water_sounds = true;
            }
        } else {
            if (_water_sounds) {
                _birds->Play(-1);
                _water_sounds = false;
            }
        }
        _music_change_timer.Stop();
    }

    if (!_freecam) {
        Vec2 campos(GetGlobalPos().x - Window::GetWidth()*0.5, GetGlobalPos().y - Window::GetHeight()*0.5);
        Window::GetCamera()->SetPos(campos);
    }
}

void Player::OnUpdate() {
    _input_timer.Start();
    if (_input_timer.GetTime() > _speed) {
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
