//
// Created by henry on 9/16/26.
//

#ifndef CIRCUITSIM_BINARYLOGICGATE_H
#define CIRCUITSIM_BINARYLOGICGATE_H

#include "../electronics.h"

class BinaryLogicGate : public IElectronic {
public:

    using IElectronic::IElectronic;

    void create() override;
    size_t numInputs() override { return 2; }
    size_t numOutputs() override { return 1; }

    void evaluate(CircuitRunner* circuit, std::vector<PinState> const& inputs, std::vector<PinState> &outputs) override;

protected:

    virtual std::string texture() = 0;
    virtual PinState logic(PinState a, PinState b) = 0;
};

#endif //CIRCUITSIM_BINARYLOGICGATE_H
