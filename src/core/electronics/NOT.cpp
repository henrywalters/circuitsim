//
// Created by henry on 9/14/26.
//
#include "NOT.h"
#include "../../common/sizes.h"
#include "../../components/pin.h"
#include <hagame/core/assets.h>
#include <hagame/graphics/components/sprite.h>
#include <hagame/math/components/rectCollider.h>


using namespace hg;

void NOTGate::create() {
    auto gate = entity->addComponent<graphics::Sprite>();
    gate->texture.path("textures/circuits/NOT");
    gate->size = LOGIC_GATE_SIZE;
    gate->centered = true;
    gate->batched = false;

    const float pinOffset = (LOGIC_GATE_SIZE[0] + PIN_THICKNESS) / 2.0 + PIN_RADIUS;

    auto left = entity->add();
    left->transform.position[0] = -pinOffset;
    left->addComponent<PinComponent>(PinType::Input, 0);

    auto right = entity->add();
    right->transform.position[0] = pinOffset;
    right->addComponent<PinComponent>(PinType::Output, 0);

    auto collider = entity->addComponent<hg::math::components::RectCollider>();
    collider->size = hg::Vec2(LOGIC_GATE_SIZE[0], LOGIC_GATE_SIZE[1]);
    collider->centered = true;
}

void NOTGate::evaluate(std::vector<PinState> const &inputs, std::vector<PinState> &outputs) {
    outputs[0] = inputs[0] == PinState::HIGH ? PinState::LOW : PinState::HIGH;
}
