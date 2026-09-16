//
// Created by henry on 9/12/26.
//

#ifndef CIRCUITSIM_SIZES_H
#define CIRCUITSIM_SIZES_H

#include <hagame/utils/aliases.h>

constexpr float GRID_SIZE = 0.1;
constexpr float GRID_THICKNESS = 0.01;

constexpr float PIN_RADIUS = 0.025;
constexpr float PIN_THICKNESS = 0.005;
constexpr int PIN_DIVISIONS = 50;
constexpr float LIGHT_THICKNESS = 0.01;

constexpr float circleRadius(int targetCells) {
    return GRID_SIZE * static_cast<float>(targetCells) - PIN_RADIUS - (PIN_THICKNESS + LIGHT_THICKNESS) / 2.0;
}

constexpr float rectWidth(int targetCells) {
    return (GRID_SIZE * static_cast<float>(targetCells / 2) - PIN_RADIUS - (PIN_THICKNESS / 2.0)) * 2.0;
}

constexpr float LIGHT_RADIUS = circleRadius(1);

constexpr int LIGHT_DIVISIONS = 50;

constexpr float WIRE_THICKNESS = 0.02;

const hg::Vec2 DC_SOURCE_SIZE {rectWidth(6), 3 * GRID_SIZE };
const hg::Vec2 LOGIC_GATE_SIZE { rectWidth(6), 3 * GRID_SIZE};
const hg::Vec2 CLOCK_SIZE { rectWidth(6), 3 * GRID_SIZE};

#endif //CIRCUITSIM_SIZES_H
