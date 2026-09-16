//
// Created by henry on 9/10/26.
//

#ifndef CIRCUITSIM_LEAD_H
#define CIRCUITSIM_LEAD_H

#include <hagame/core/entity.h>
#include <hagame/core/component.h>
#include "../common/colors.h"

class LeadComponent : public hg::Component {
public:

    std::vector<hg::Entity*> connectedTo;

protected:

    OBJECT_NAME(LeadComponent)

};

HG_COMPONENT(Electronics, LeadComponent)

#endif //CIRCUITSIM_PIN_H
