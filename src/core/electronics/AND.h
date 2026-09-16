//
// Created by henry on 9/14/26.
//

#ifndef CIRCUITSIM_AND_H
#define CIRCUITSIM_AND_H

#include "../electronics.h"

class ANDGate : public IElectronic {
public:

    using IElectronic::IElectronic;

    void create() override;

    size_t numInputs() override {
        return 2;
    }
    size_t numOutputs() override {
        return 1;
    }

    void evaluate(CircuitRunner* circuit, std::vector<PinState> const& inputs, std::vector<PinState>& outputs) override;

};

CS_COMPONENT(ANDGate)

#endif //CIRCUITSIM_AND_H
