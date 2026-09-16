//
// Created by henry on 9/11/26.
//

#ifndef CIRCUITSIM_DCSOURCE_H
#define CIRCUITSIM_DCSOURCE_H

#include "../electronics.h"

class DCSource : public IElectronic {
public:

    using IElectronic::IElectronic;

    void create() override;
    size_t numInputs() override {
        return 1;
    }
    size_t numOutputs() override {
        return 1;
    }

    void evaluate(CircuitRunner* circuit, std::vector<PinState> const& inputs, std::vector<PinState> &outputs) override;
};

CS_COMPONENT(DCSource)

#endif //CIRCUITSIM_DCSOURCE_H
