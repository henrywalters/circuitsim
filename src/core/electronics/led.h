//
// Created by henry on 9/10/26.
//

#ifndef CIRCUITSIM_LED_H
#define CIRCUITSIM_LED_H

#include "../electronics.h"

class LED : public IElectronic {
public:

    using IElectronic::IElectronic;

    void create() override;

    size_t numInputs() override {
        return 1;
    }
    size_t numOutputs() override {
        return 1;
    }

    void evaluate(CircuitRunner* circuit, std::vector<PinState> const& inputs, std::vector<PinState>& outputs) override;

};

CS_COMPONENT(LED);

#endif //CIRCUITSIM_LED_H
