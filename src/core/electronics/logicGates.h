//
// Created by henry on 9/16/26.
//

#ifndef CIRCUITSIM_LOGICGATES_H
#define CIRCUITSIM_LOGICGATES_H

#include "binaryLogicGate.h"
#include "binaryLogicGates.h"
#include "unaryLogicGate.h"

class ANDGate : public BinaryLogicGate {
public:
    using BinaryLogicGate::BinaryLogicGate;
protected:
    std::string texture() override { return "textures/circuits/AND"; }
    PinState logic(PinState a, PinState b) override {
        return a == PinState::HIGH && b == PinState::HIGH ? PinState::HIGH : PinState::LOW;
    }
};

class ORGate : public BinaryLogicGate {
public:
    using BinaryLogicGate::BinaryLogicGate;
protected:
    std::string texture() override { return "textures/circuits/OR"; }
    PinState logic(PinState a, PinState b) override {
        return a == PinState::HIGH || b == PinState::HIGH ? PinState::HIGH : PinState::LOW;
    }
};

class NANDGate : public BinaryLogicGate {
public:
    using BinaryLogicGate::BinaryLogicGate;
protected:
    std::string texture() override { return "textures/circuits/NAND"; }
    PinState logic(PinState a, PinState b) override {
        return a == PinState::HIGH && b == PinState::HIGH ? PinState::LOW : PinState::HIGH;
    }
};

class XORGate : public BinaryLogicGate {
public:
    using BinaryLogicGate::BinaryLogicGate;
protected:
    std::string texture() override { return "textures/circuits/XOR"; }
    PinState logic(PinState a, PinState b) override {
        if (a == PinState::LOW && b == PinState::LOW) outputs[0] = PinState::LOW;
        else if (a == PinState::LOW && b == PinState::HIGH) outputs[0] = PinState::HIGH;
        else if (b == PinState::LOW && a == PinState::HIGH) outputs[0] = PinState::HIGH;
        else outputs[0] = PinState::LOW;
    }
};

class NOTGate : public UnaryLogicGate {
public:
    using UnaryLogicGate::UnaryLogicGate;
protected:
    std::string texture() override { return "textures/circuits/NOT"; }
    PinState logic(PinState a) override {
        return a == PinState::HIGH ? PinState::LOW : PinState::HIGH;
    }
};

CS_COMPONENT(ANDGate)
CS_COMPONENT(ORGate)
CS_COMPONENT(NANDGate)
CS_COMPONENT(XORGate)
CS_COMPONENT(NOTGate)

#endif //CIRCUITSIM_LOGICGATES_H
