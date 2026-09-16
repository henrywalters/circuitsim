//
// Created by henry on 9/14/26.
//

#ifndef CIRCUITSIM_NOT_H
#define CIRCUITSIM_NOT_H

#include "../electronics.h"

class NOTGate : public IElectronic {
public:

    using IElectronic::IElectronic;

    void create() override;
    size_t numInputs() override {
        return 1;
    }
    size_t numOutputs() override {
        return 1;
    }

    void evaluate(std::vector<PinState> const& inputs, std::vector<PinState> &outputs) override;

};

CS_COMPONENT(NOTGate)

#endif //CIRCUITSIM_AND_H
