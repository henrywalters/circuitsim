//
// Created by henry on 9/16/26.
//
#include "clock.h"

#include <hagame/graphics/components/sprite.h>
#include <hagame/math/components/rectCollider.h>

#include "../../components/pin.h"
#include "../../common/sizes.h"

using namespace hg;

void Clock::create() {
    auto chip = entity->addComponent<hg::graphics::Sprite>();
    chip->centered = true;
    chip->texture.path("textures/circuits/CLOCK");
    chip->size = CLOCK_SIZE;

    const float pinOffset = (CLOCK_SIZE[0] + PIN_THICKNESS) / 2.0 + PIN_RADIUS;

    auto left = entity->add();
    left->transform.position[0] = -pinOffset;;
    left->addComponent<PinComponent>(PinType::Input, 0);

    auto right = entity->add();
    right->transform.position[0] = pinOffset;
    right->addComponent<PinComponent>(PinType::Output, 0);

    auto collider = entity->addComponent<hg::math::components::RectCollider>();
    collider->size = hg::Vec2(CLOCK_SIZE[0], CLOCK_SIZE[1]);
    collider->centered = true;
}

void Clock::onClockEdge(CircuitRunner* circuit, ClockEdge edge, std::vector<PinState> const &inputs, std::vector<PinState> &outputs) {
    m_state = (m_state == PinState::HIGH) ? PinState::LOW : PinState::HIGH;
    outputs[0] = m_state;
    circuit->addEvent(Event::Type::ClockTick, this, circuit->time() + m_period);
}
