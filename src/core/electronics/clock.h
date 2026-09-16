//
// Created by henry on 9/16/26.
//

#ifndef CIRCUITSIM_CLOCK_H
#define CIRCUITSIM_CLOCK_H

#include "../electronics.h"

class Clock : public IElectronic {
public:

    using IElectronic::IElectronic;

    void create() override;

    size_t numInputs() override { return 1;}
    size_t numOutputs() override { return 1; }

    void onClockEdge(CircuitRunner* circuit, ClockEdge edge, std::vector<PinState> const &inputs, std::vector<PinState> &outputs) override;

private:

    PinState m_state = PinState::LOW;
    uint64_t m_period = 1e8;

};

CS_COMPONENT(Clock)

#endif //CIRCUITSIM_CLOCK_H
