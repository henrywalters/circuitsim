//
// Created by henry on 9/13/26.
//

#ifndef CIRCUITSIM_ELECTRONIC_COMPONENT_H
#define CIRCUITSIM_ELECTRONIC_COMPONENT_H

#include <hagame/core/entity.h>
#include <hagame/core/component.h>

class ElectronicComponent : public hg::Component {
public:

    std::string name;

protected:

    OBJECT_NAME(ElectronicComponent)

};

HG_COMPONENT(Electronics, ElectronicComponent)
HG_FIELD(ElectronicComponent, std::string, name)

#endif //CIRCUITSIM_ELECTRONIC_H
