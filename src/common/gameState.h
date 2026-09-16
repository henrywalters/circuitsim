//
// Created by henry on 2/29/24.
//

#ifndef HABAN_GAMESTATE_H
#define HABAN_GAMESTATE_H

#include <hagame/utils/random.h>
#include <hagame/utils/enum.h>
#include <hagame/input/input.h>
#include <hagame/utils/variantMap.h>

#include "../constants.h"
#include "../common/enums/actions.h"
#include "../common/persistentSettings.h"

// Access GameState via the singleton Get function
class GameState {
public:

    bool running = true;
    bool editing = false;

    double elapsedTime = 0.0;
    bool hasCamera;

    hg::Vec2 rawMousePos;
    hg::Vec2 mousePos;
    hg::Vec2 dragPos;

    float pixelsPerMeter = 1.0f;

    PersistentSettings persistentSettings;

    hg::input::Actions<hg::utils::enum_t> input;
    hg::utils::Random random;

    static GameState* Get();

};

#endif //HABAN_GAMESTATE_H
