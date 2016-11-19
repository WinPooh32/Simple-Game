#include "Player.h"

Player::Player(Map* map) {
    _speed = TREE_SPEED;
    _water_sounds = false;
    _map = map;
    _side = SIDE_LEFT;

    _sound_mgr = new SoundManager(_map);
    
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
}

Player::~Player(){
    delete _sound_mgr;
}

void Player::MoveIt(const Vec2& offset) {

    tile mtile = _map->GetTile(GetGlobalPos() + offset);
    if (_map->CanMove(mtile)) {
        if (mtile == TILE_TREE || mtile == TILE_TREE_SPRUCE) {
            _speed = TREE_SPEED;
        } else {
            _speed = GRASS_SPEED;
        }
        Move(offset);

        _sound_mgr->Update(GetGlobalPos());
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
                //_waves->Play(-1);
                _water_sounds = true;
            }
        } else {
            if (_water_sounds) {
                //_birds->Play(-1);
                _water_sounds = false;
            }
        }
        _music_change_timer.Stop();
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

        if (!_freecam) {

            Vec2 cam_pos = Window::GetCamera()->GetPos();
            Vec2 cam_view = Window::GetCamera()->GetViewport();

            if(GetGlobalPos().x * Map::SCALED_TILE_SIZE < cam_pos.x + cam_view.x*0.2 ){
               cam_pos += Vec2::Left*Map::SCALED_TILE_SIZE;
            }
            else if(GetGlobalPos().x * Map::SCALED_TILE_SIZE > (cam_pos.x + cam_view.x) - cam_view.x*0.2){
                cam_pos += Vec2::Right*Map::SCALED_TILE_SIZE;
            }


            if(GetGlobalPos().y * Map::SCALED_TILE_SIZE < cam_pos.y + cam_view.y*0.2 ){
               cam_pos += Vec2::Up*Map::SCALED_TILE_SIZE;
            }
            else if(GetGlobalPos().y * Map::SCALED_TILE_SIZE > (cam_pos.y + cam_view.y) - cam_view.y*0.2){
                cam_pos += Vec2::Down*Map::SCALED_TILE_SIZE;
            }

            Window::GetCamera()->SetPos(cam_pos);

            //Vec2 campos(GetGlobalPos().x - Window::GetWidth()*0.5, GetGlobalPos().y - Window::GetHeight()*0.5);
           // Window::GetCamera()->SetPos(campos);
        }

        _input_timer.Stop();
    }

    _sound_mgr->ProcessFading();
}

void Player::OnRender() {
    _sprite.Draw(GetGlobalPos() * Map::SCALED_TILE_SIZE, Vec2(Map::SCALED_TILE_SIZE, Map::SCALED_TILE_SIZE), Window::GetCamera());
}
