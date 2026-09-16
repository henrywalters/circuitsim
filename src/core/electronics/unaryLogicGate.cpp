//
// Created by henry on 9/16/26.
//
#include "unaryLogicGate.h"
#include "../../common/sizes.h"
#include "../../components/pin.h"
#include <hagame/core/assets.h>
#include <hagame/graphics/components/sprite.h>
#include <hagame/math/components/rectCollider.h>

using namespace hg;

void UnaryLogicGate::create() {
    auto gate = entity->addComponent<graphics::Sprite>();
    gate->texture.path(texture());
    gate->size = LOGIC_GATE_SIZE;
    gate->centered = true;
    gate->batched = false;

    const float pinOffset = (LOGIC_GATE_SIZE[0] + PIN_THICKNESS) / 2.0 + PIN_RADIUS;

    auto left = entity->add();
    left->transform.position[0] = -pinOffset;
    left->transform.position[1] = -GRID_SIZE;
    left->addComponent<PinComponent>(PinType::Input, 0);

    auto right = entity->add();
    right->transform.position[0] = pinOffset;
    right->addComponent<PinComponent>(PinType::Output, 0);

    auto collider = entity->addComponent<hg::math::components::RectCollider>();
    collider->size = hg::Vec2(LOGIC_GATE_SIZE[0], LOGIC_GATE_SIZE[1]);
    collider->centered = true;
}

void UnaryLogicGate::evaluate(CircuitRunner* circuit, std::vector<PinState> const &inputs, std::vector<PinState> &outputs) {
    outputs[0] = logic(inputs[0]);
}
