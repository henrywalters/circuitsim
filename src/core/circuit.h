//
// Created by henry on 9/13/26.
//

#ifndef CIRCUITSIM_CIRCUIT_H
#define CIRCUITSIM_CIRCUIT_H

#include <queue>
#include <hagame/core/entity.h>

#include "events.h"
#include "state.h"

class IElectronic;

struct CompiledComponent {
    IElectronic* electronic;
    std::vector<size_t> inputNets;
    std::vector<size_t> outputNets;
};

struct CompiledCircuit {
    std::vector<PinState> netStates;
    std::vector<CompiledComponent> components;
    std::vector<std::vector<size_t>> netSubscribers;
};

class CircuitRunner {
public:

    void compile(hg::EntityManager& entities);
    void tick(uint64_t virtualTimeBudget);
    void notifyChange(IElectronic* electronic);
    size_t getComponentIndex(IElectronic* electronic);
    uint64_t time() { return m_time; }
    void addEvent(Event::Type type, IElectronic* electronic, uint64_t time);

private:

    void driveNet(size_t netIndex, PinState newValue, uint64_t scheduleAt);
    Events m_events;
    CompiledCircuit m_circuit;
    uint64_t m_time = 0;


};

#endif //CIRCUITSIM_CIRCUIT_H
