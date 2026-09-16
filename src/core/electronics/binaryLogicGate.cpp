//
// Created by henry on 9/16/26.
//
#include "binaryLogicGate.h"
#include "../../common/sizes.h"
#include "../../components/pin.h"
#include <hagame/core/assets.h>
#include <hagame/graphics/components/sprite.h>
#include <hagame/math/components/rectCollider.h>

using namespace hg;

void BinaryLogicGate::create() {
    auto gate = entity->addComponent<graphics::Sprite>();
    gate->texture.path(texture());
    gate->size = LOGIC_GATE_SIZE;
    gate->centered = true;
    gate->batched = false;

    const float pinOffset = (LOGIC_GATE_SIZE[0] + PIN_THICKNESS) / 2.0 + PIN_RADIUS;

    auto left1 = entity->add();
    left1->transform.position[0] = -pinOffset;
    left1->transform.position[1] = -GRID_SIZE;
    left1->addComponent<PinComponent>(PinType::Input, 0);

    auto left2 = entity->add();
    left2->transform.position[0] = -pinOffset;
    left2->transform.position[1] = GRID_SIZE;
    left2->addComponent<PinComponent>(PinType::Input, 1);

    auto right = entity->add();
    right->transform.position[0] = pinOffset;
    right->addComponent<PinComponent>(PinType::Output, 0);

    auto collider = entity->addComponent<hg::math::components::RectCollider>();
    collider->size = hg::Vec2(LOGIC_GATE_SIZE[0], LOGIC_GATE_SIZE[1]);
    collider->centered = true;
}

void BinaryLogicGate::evaluate(CircuitRunner* circuit, std::vector<PinState> const &inputs, std::vector<PinState> &outputs) {
    outputs[0] = logic(inputs[0], inputs[1]);
}
