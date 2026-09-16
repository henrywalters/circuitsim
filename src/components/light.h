//
// Created by henry on 9/10/26.
//

#ifndef CIRCUITSIM_LIGHT_H
#define CIRCUITSIM_LIGHT_H

#include <hagame/core/entity.h>
#include <hagame/core/component.h>

#include "../common/colors.h"


class LightComponent : public hg::Component {
public:

    hg::graphics::Color color = hg::graphics::Color::red();
    bool on = false;

protected:

    OBJECT_NAME(LightComponent)

};

HG_COMPONENT(Electronics, LightComponent)
HG_FIELD(LightComponent, hg::graphics::Color, color)
HG_FIELD(LightComponent, bool, on)

#endif //CIRCUITSIM_LED_H
