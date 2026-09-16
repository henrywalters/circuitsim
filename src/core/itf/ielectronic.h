//
// Created by henry on 9/10/26.
//

#ifndef CIRCUITSIM_IELECTRONIC_H
#define CIRCUITSIM_IELECTRONIC_H

#include <hagame/core/entity.h>

#include "../circuit.h"
#include "../state.h"

class IElectronic {
public:

    IElectronic(hg::Entity* entity):
        entity(entity) {}

    hg::Entity* entity;

    virtual ~IElectronic() = default;
    virtual void create() = 0;

    virtual size_t numInputs() = 0;
    virtual size_t numOutputs() = 0;

    virtual void evaluate(CircuitRunner* circuit, std::vector<PinState> const& inputs, std::vector<PinState>& outputs) {}
    virtual void onClockEdge(CircuitRunner* circuit, ClockEdge edge, std::vector<PinState> const& inputs, std::vector<PinState>& outputs) {}
};

HG_IS_CONCEPT(IElectronic)

#endif //CIRCUITSIM_IELECTRONIC_H
