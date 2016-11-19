#include "SoundManager.h"

#define AUDIO(x) (Resources::GetAudio(x))


//enum tile {
//    TILE_GRASS = 0,
//    TILE_WATER,
//    TILE_PLAIN_GRASS,
//    TILE_TREE,
//    TILE_MOUNTAIN,
//    TILE_TREE_SPRUCE,
//    TILE_OBJECT,
//    TILE_BUILDING,
//    TILE_NONE // END OF TILES
//};

SoundManager::SoundManager(Map *map){
    _map = map;

    _Sounds.push_back({AUDIO("field.ogg")}); //GRASS
    _Sounds.push_back({AUDIO("waves.ogg")});//WATER
    _Sounds.push_back({AUDIO("birds.ogg")}); //PLAINGRASS
    _Sounds.push_back({AUDIO("spruce_forest.ogg")});//TREE;
    _Sounds.push_back({AUDIO("mountains_wind.ogg")});//MOUNTAIN
}

void SoundManager::ProcessFading(){
    timer.Start();

    if(timer.GetTime() > 100){
        if(_Fade.empty()){
            return;
        }

        auto last = --_Fade.end();
        for(auto it = last; it >= _Fade.begin(); --it){

            if(it->is_fadein){
                it->audio->SetVolume(it->audio->GetVolume() + FADE_SPEED);

                if(it->audio->GetVolume() >= 100){
                    _Fade.erase(it);
                }
            }
            else{
                it->audio->SetVolume(it->audio->GetVolume() - FADE_SPEED);

                if(it->audio->GetVolume() <= 0){
                    it->audio->Stop();
                    _Fade.erase(it);
                }
            }

        }

        timer.Stop();
    }
}

 std::vector<SoundManager::FadingState>::iterator SoundManager::searchfor(std::vector<FadingState>& vect, Audio *audio){
    auto end = vect.end();

    for(auto it = vect.begin(); it != end; ++it){
        if(it->audio == audio){
            return it;
        }
    }

    return end;
}

void SoundManager::FadeIn(Audio *audio, int loops){
    auto it = searchfor(_Fade, audio);
    if(it == _Fade.end()){
        audio->SetVolume(0);
        audio->Play(loops);
        _Fade.push_back({audio, true});
    }else{
        it->is_fadein = true;

//        if(!audio->IsPlaying()){
//            audio->Play(loops);
//        }
    }
}

void SoundManager::FadeOut(Audio *audio){
    auto it = searchfor(_Fade, audio);
    if(it == _Fade.end()){
        _Fade.push_back({audio, false});
    }else{
        it->is_fadein = false;
    }
}

void SoundManager::Update(const Vec2& player_pos){

    for(int chk_tile = TILE_GRASS; chk_tile < TILE_NONE && chk_tile < _Sounds.size(); chk_tile++){

        //Vec2 offset = GetBiomeSoundCenter(static_cast<tile>(chk_tile), player_pos, in_view);
        bool playing = _Sounds[chk_tile][0]->IsPlaying();
        bool null_vol = _Sounds[chk_tile][0]->GetVolume() == 0;
        bool near = IsNear(static_cast<tile>(chk_tile), player_pos);
        if(near){
            if(!playing){
                FadeIn(_Sounds[chk_tile][0], -1);
            }
            //_Sounds[chk_tile][0]->SetPanning(offset + viewport*0.5f, viewport, size);
        }else{
            if(playing){
                FadeOut(_Sounds[chk_tile][0]);
            }
        }
    }
}

Vec2 SoundManager::GetBiomeSoundCenter(tile biome,const Vec2& map_point, bool& exists ){
//std::cout << map_point.x << " " << map_point.y << std::endl;


    Vec2 offset;
    exists = true;

    //============ calc Y ==========
    float sum = 0;
    int count = 0;

    for(int i = -3; i <= 3; ++i){

        float row_sum = 0;
        int row_count = 0;

        for(int j = -3; j <= 3; ++j){
            if(_map->GetTileNear(map_point, Vec2(i, j)) == biome){
                    ++row_count;
                    row_sum += j;
            }
        }

        if(row_count > 0){
            ++count;
            sum += row_sum / row_count;
        }

    }

    if(count > 0){
        offset.y = sum / count;
    }else{
        exists = false;
    }

    //============ calc X ==========
    sum = 0;
    count = 0;

    for(int i = -3; i <= 3; ++i){

        float row_sum = 0;
        int row_count = 0;

        for(int j = -3; j <= 3; ++j){
            if(_map->GetTileNear(map_point, Vec2(j, i)) == biome){
                    ++row_count;
                    row_sum += j;
            }
        }

        if(row_count > 0){
            ++count;
            sum += row_sum / row_count;
        }

    }

    if(count > 0){
        offset.x = sum / count;
    }else{
        exists += false;
    }

    return offset;
}

bool SoundManager::IsNear(tile biome, const Vec2& point){
    for(int i = -3; i <= 3; ++i){
        for(int j = -3; j <= 3; ++j){
            if(_map->GetTileNear(point, Vec2(i, j)) == biome){
                return true;
            }
        }
    }

    return false;
}
