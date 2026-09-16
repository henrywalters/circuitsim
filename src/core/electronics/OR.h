//
// Created by henry on 9/14/26.
//

#ifndef CIRCUITSIM_OR_H
#define CIRCUITSIM_OR_H

#include "../electronics.h"

class ORGate : public IElectronic {
public:

    using IElectronic::IElectronic;

    void create() override;
    size_t numInputs() override {
        return 2;
    }
    size_t numOutputs() override {
        return 1;
    }

    void evaluate(std::vector<PinState> const& inputs, std::vector<PinState> &outputs) override;

};

CS_COMPONENT(ORGate)

#endif //CIRCUITSIM_AND_H
