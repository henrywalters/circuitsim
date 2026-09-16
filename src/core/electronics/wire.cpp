//
// Created by henry on 9/12/26.
//
#include "wire.h"

#include "../../components/wire.h"
#include "../../components/lead.h"

using namespace hg;

Entity* addWire(Entity *parent) {
    auto wire = parent->add();
    wire->addComponent<WireComponent>();
    wire->add()->addComponent<LeadComponent>();
    wire->add()->addComponent<LeadComponent>();
    return wire;
}

WireLeads getWireLeads(hg::Entity *entity) {
    if (entity->hasComponent<WireComponent>()) {
        auto leads = entity->children();
        return WireLeads {
        static_cast<Entity*>(leads[0])->getComponent<LeadComponent>(),
        static_cast<Entity*>(leads[1])->getComponent<LeadComponent>(),
        };
    } else {
        throw std::runtime_error("Entity is not a wire");
    }
}
