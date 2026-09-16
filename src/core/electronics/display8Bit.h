//
// Created by henry on 9/13/26.
//

#ifndef CIRCUITSIM_DISPLAY8BIT_H
#define CIRCUITSIM_DISPLAY8BIT_H
#include "../electronics.h"

class Display8Bit: public IElectronic {
public:

    using IElectronic::IElectronic;

    void create() override;
    size_t numInputs() override {
        return 1;
    }
    size_t numOutputs() override {
        return 1;
    }

};

CS_COMPONENT(Display8Bit);

#endif //CIRCUITSIM_DISPLAY8BIT_H
