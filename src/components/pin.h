//
// Created by henry on 9/10/26.
//

#ifndef CIRCUITSIM_PIN_H
#define CIRCUITSIM_PIN_H

#include <hagame/core/entity.h>
#include <hagame/core/component.h>
#include "../common/colors.h"

ENUM(PinType)
ENUM_VALUE(PinType, Input)
ENUM_VALUE(PinType, Output)

class PinComponent : public hg::Component {
public:

    PinComponent() = default;

    PinComponent(PinType::type type, int index):
        type(type),
        index(index)
    {}

    std::vector<hg::Entity*> connectedTo;
    PinType::type type;
    int index = 0;

protected:

    OBJECT_NAME(PinComponent)

};

HG_COMPONENT(Electronics, PinComponent)
HG_FIELD(PinComponent, hg::utils::uuid_t, type)
HG_FIELD(PinComponent, int, index)

#endif //CIRCUITSIM_PIN_H
