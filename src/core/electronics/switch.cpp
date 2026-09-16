//
// Created by henry on 9/14/26.
//
#include "switch.h"

#include "../../components/switch.h"
#include "../../components/pin.h"
#include "../../common/sizes.h"

#include <hagame/graphics/components/sprite.h>
#include <hagame/math/components/rectCollider.h>

using namespace hg;

void Switch::create() {
    entity->addComponent<SwitchComponent>()->on = true;
    auto sprite = entity->addComponent<graphics::Sprite>();
    sprite->texture.path("textures/circuits/switch_on");
    sprite->size = LOGIC_GATE_SIZE;
    sprite->centered = true;
    sprite->batched = false;

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

void Switch::evaluate(std::vector<PinState> const &inputs, std::vector<PinState> &outputs) {
    auto switchComp = entity->getComponent<SwitchComponent>();
    outputs[0] = inputs[0] == PinState::HIGH && switchComp->on ? PinState::HIGH : PinState::LOW;
}
