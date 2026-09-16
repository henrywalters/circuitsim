//
// Created by henry on 9/10/26.
//
#include "led.h"

#include "../../components/light.h"
#include "../../components/pin.h"
#include "../../common/sizes.h"

#include <hagame/math/components/rectCollider.h>

using namespace hg;
using namespace hg::graphics;

void LED::create() {
    entity->addComponent<LightComponent>();
    auto left = entity->add();

    const auto pinOffset = LIGHT_RADIUS + PIN_RADIUS + (PIN_THICKNESS + LIGHT_THICKNESS) / 2;

    left->transform.position[0] = -pinOffset;
    left->addComponent<PinComponent>(PinType::Input, 0);

    auto right = entity->add();
    right->transform.position[0] = pinOffset;
    right->addComponent<PinComponent>(PinType::Output, 0);

    auto collider = entity->addComponent<hg::math::components::RectCollider>();
    collider->centered = true;
    collider->size = Vec2{LIGHT_RADIUS * 2, LIGHT_RADIUS * 2};
}

void LED::evaluate(CircuitRunner* circuit, std::vector<PinState> const &inputs, std::vector<PinState> &outputs) {
    outputs[0] = inputs[0];

    auto light = entity->getComponent<LightComponent>();

    light->on = outputs[0] == PinState::HIGH;
}
