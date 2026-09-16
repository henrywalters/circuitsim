//
// Created by henry on 9/12/26.
//

#ifndef CIRCUITSIM_WIRE_COMPONENT_H
#define CIRCUITSIM_WIRE_COMPONENT_H

#include <hagame/core/entity.h>
#include <hagame/core/component.h>
#include "../common/colors.h"

class WireComponent : public hg::Component {
protected:

    OBJECT_NAME(WireComponent)

};

HG_COMPONENT(Electronics, WireComponent)

#endif //CIRCUITSIM_WIRE_H
