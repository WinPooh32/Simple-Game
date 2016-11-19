#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <vector>

#include <Core/Audio.h>
#include <Core/Resources.h>
#include <Core/Timer.h>

#include "Map.h"

class SoundManager{
public:

//    enum Biome{
//        FIELD = 0,
//        FOREST,
//        MOUNTAINS,
//        SEA,
//        RIVER,
//        SWAMP
//    };

    const int MAX_SOURCES = 4;
    const int MAX_DISTANCE = 3;

    SoundManager(Map* map);

    void ProcessFading();

    void FadeIn(Audio* audio, int loops);
    void FadeOut(Audio* audio);

    void Update(const Vec2& player_pos);

    Vec2 GetBiomeSoundCenter(tile biome, const Vec2 &map_point, bool &exists);
    bool IsNear(tile biome, const Vec2 &point);

private:
    const int FADE_SPEED = 5;// 2 vols per 100ms
    struct FadingState{
        Audio* audio;
        bool is_fadein;
    };

    Timer timer;
    Map* _map;
    std::vector<std::vector<Audio*>> _Sounds;
    std::vector<FadingState> _Fade;

    std::vector<FadingState>::iterator searchfor(std::vector<FadingState>& vect, Audio *aud);
};

#endif // SOUNDMANAGER_H
