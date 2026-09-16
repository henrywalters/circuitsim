//
// Created by henry on 9/11/26.
//

#include "dcSource.h"

#include <hagame/graphics/components/quad.h>
#include <hagame/graphics/components/sprite.h>
#include <hagame/math/components/rectCollider.h>

#include "../../common/colors.h"
#include "../../common/sizes.h"
#include "../../components/pin.h"

void DCSource::create() {
    auto chip = entity->addComponent<hg::graphics::Sprite>();
    chip->centered = true;
    chip->texture.path("textures/circuits/dc_source");
    chip->size = DC_SOURCE_SIZE;

    const float pinOffset = (DC_SOURCE_SIZE[0] + PIN_THICKNESS) / 2.0 + PIN_RADIUS;

    auto left = entity->add();
    left->transform.position[0] = -pinOffset;;
    left->addComponent<PinComponent>(PinType::Input, 0);

    auto right = entity->add();
    right->transform.position[0] = pinOffset;
    right->addComponent<PinComponent>(PinType::Output, 0);

    auto collider = entity->addComponent<hg::math::components::RectCollider>();
    collider->size = hg::Vec2(DC_SOURCE_SIZE[0], DC_SOURCE_SIZE[1]);
    collider->centered = true;
}

void DCSource::evaluate(CircuitRunner* circuit, std::vector<PinState> const& inputs, std::vector<PinState> &outputs) {
    outputs[0] = PinState::HIGH;
}
