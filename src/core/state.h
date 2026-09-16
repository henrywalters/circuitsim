//
// Created by henry on 9/16/26.
//

#ifndef CIRCUITSIM_STATE_H
#define CIRCUITSIM_STATE_H
#include <cstdint>

enum class PinState : uint8_t {
    LOW,
    HIGH,
    HI_Z,
    UNDEFINED,
};

enum class ClockEdge : uint8_t {
    Rising,
    Falling,
};


#endif //CIRCUITSIM_STATE_H
