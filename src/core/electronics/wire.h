//
// Created by henry on 9/12/26.
//

#ifndef CIRCUITSIM_WIRE_H
#define CIRCUITSIM_WIRE_H

#include "../../components/lead.h"

struct WireLeads {
    LeadComponent* a;
    LeadComponent* b;
};

hg::Entity* addWire(hg::Entity* parent);
WireLeads getWireLeads(hg::Entity* entity);


#endif //CIRCUITSIM_WIRE_H
