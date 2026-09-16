//
// Created by henry on 9/14/26.
//

#ifndef CIRCUITSIM_SWITCH_COMP_H
#define CIRCUITSIM_SWITCH_COMP_H

#include <hagame/core/entity.h>
#include <hagame/core/component.h>
#include <hagame/graphics/components/sprite.h>

class SwitchComponent : public hg::Component {
public:

    bool on = true;

    void toggle() {
        on = !on;
        entity->getComponent<hg::graphics::Sprite>()->texture.path(on ? "textures/circuits/switch_on" : "textures/circuits/switch_off");
    }

protected:

    OBJECT_NAME(SwitchComponent)

};

HG_COMPONENT(Electronics, SwitchComponent)
HG_FIELD(SwitchComponent, bool, on)

#endif //CIRCUITSIM_SWITCH_H
