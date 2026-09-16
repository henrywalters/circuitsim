//
// Created by henry on 9/13/26.
//
#include "circuit.h"

#include <set>

#include "electronics.h"
#include "../components/electronic.h"
#include "../components/lead.h"
#include "../components/wire.h"
#include "../components/pin.h"
#include "electronics/wire.h"

using namespace hg;

struct UnionFind {
    std::vector<size_t> parent;

    UnionFind(size_t n): parent(n) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    size_t find(size_t x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    }

    void unite(size_t a, size_t b) {
        parent[find(a)] = find(b);
    }
};

void CircuitRunner::compile(hg::EntityManager& entities) {

    m_circuit.components.clear();
    m_circuit.netSubscribers.clear();
    m_circuit.netStates.clear();

    m_time = 0;

    while (!m_events.queue.empty()) {
        m_events.queue.pop();
    }

    std::vector<size_t> allPins;
    std::vector<IElectronic*> pinOwner;
    std::vector<std::pair<size_t, size_t>> wirePairs;
    std::unordered_map<IElectronic*, size_t> basePinIndex;

    std::unordered_map<IElectronic*, size_t> componentIndex;

    entities.forEach<ElectronicComponent>([&](ElectronicComponent* comp, Entity* entity) {
        auto instance = ElectronicsFactory::GetInstance(entity->id());
        basePinIndex[instance] = allPins.size();
        auto numIn = instance->numInputs();
        auto numOut = instance->numOutputs();
        for (size_t i = 0; i < numIn + numOut; i++) {
            pinOwner.push_back(instance);
            allPins.push_back(allPins.size());
        }

        componentIndex[instance] = m_circuit.components.size();
        m_circuit.components.push_back(CompiledComponent{
            .electronic = instance,
            .inputNets = std::vector<size_t>(numIn, -1),
            .outputNets = std::vector<size_t>(numOut, -1),
        });
    });

    std::set<Entity*> visitedWires;

    std::function<void(Entity*, std::vector<Entity*>&)> findWirePins = [&](Entity* wire, std::vector<Entity*>& pins) {
        if (visitedWires.contains(wire)) return;
        visitedWires.insert(wire);

        auto leads = getWireLeads(wire);

        auto iterateConnections = [&](std::vector<Entity*>& connections) {
            for (const auto& connection : connections) {
                if (connection->hasComponent<PinComponent>()) {
                    if (std::find(pins.begin(), pins.end(), connection) == pins.end()) {
                        pins.push_back(connection); // only add once
                    }
                }
                auto parent = static_cast<Entity*>(connection->parent());
                if (parent->hasComponent<WireComponent>()) {
                    findWirePins(parent, pins);
                }
            }
        };

        iterateConnections(leads.a->connectedTo);
        iterateConnections(leads.b->connectedTo);
    };

    auto getPinIndex = [&](Entity* pin) {
        auto comp = pin->getComponent<PinComponent>();
        auto instance = ElectronicsFactory::GetInstance(pin->parent()->id());
        size_t localIndex = comp->index;
        if (comp->type == PinType::Output) {
            localIndex += instance->numInputs(); // match the offset convention used in compile()'s fill-in loop
        }
        return basePinIndex.at(instance) + localIndex;
    };

    entities.forEach<WireComponent>([&](WireComponent* wire, Entity* entity) {
        std::vector<Entity*> connectedPins;
        findWirePins(entity, connectedPins);

        for (size_t i = 0; i < connectedPins.size(); i++) {
            for (size_t j = i + 1; j < connectedPins.size(); j++) {
                auto pi = getPinIndex(connectedPins[i]);
                auto pj = getPinIndex(connectedPins[j]);
                std::cout << "Wire joins global pin " << pi << " <-> " << pj << "\n";
                wirePairs.push_back({
                    getPinIndex(connectedPins[i]),
                    getPinIndex(connectedPins[j])
                });
            }
        }
    });

    for (auto& [inst, base] : basePinIndex) {
        std::cout << "Component base=" << base << " numIn=" << inst->numInputs() << " numOut=" << inst->numOutputs() << "\n";
    }

    UnionFind uf(allPins.size());
    for (auto& [a, b] : wirePairs) uf.unite(a, b);

    std::unordered_map<int,int> netIdRemap;
    for (size_t i = 0; i < allPins.size(); i++) {
        auto root = uf.find(i);
        if (!netIdRemap.count(root)) {
            netIdRemap[root] = netIdRemap.size();
        }
    }

    m_circuit.netStates.resize(netIdRemap.size(), PinState::UNDEFINED);

    for (auto& [electronic, compIdx] : componentIndex) {
        int base = basePinIndex[electronic];
        auto& comp = m_circuit.components[compIdx];

        for (int i = 0; i < comp.inputNets.size(); i++) {
            auto pin = base + i;
            comp.inputNets[i] = netIdRemap[uf.find(pin)];
        }

        for (int i = 0; i < comp.outputNets.size(); i++) {
            auto pin = base + comp.inputNets.size() + i;
            comp.outputNets[i] = netIdRemap[uf.find(pin)];
        }
    }

    m_circuit.netSubscribers.assign(m_circuit.netStates.size(), {});

    for (size_t c = 0; c < m_circuit.components.size(); c++) {
        for (size_t net : m_circuit.components[c].inputNets) {
            m_circuit.netSubscribers[net].push_back(c);
        }
        m_events.queue.push(Event{0, c, Event::Type::InputChanged}); // prime unconditionally, once per component
    }
}

void CircuitRunner::addEvent(Event::Type type, IElectronic *electronic, uint64_t time) {
    m_events.queue.push(Event{
        time,
        getComponentIndex(electronic),
        type
    });
}

void CircuitRunner::driveNet(size_t netIndex, PinState newValue, uint64_t scheduleAt) {
    if (m_circuit.netStates[netIndex] == newValue) return;

    m_circuit.netStates[netIndex] = newValue;

    for (auto sub : m_circuit.netSubscribers[netIndex]) {
        m_events.queue.push(Event{
            scheduleAt,
            sub,
            Event::Type::InputChanged
        });
    }
}

void CircuitRunner::tick(uint64_t virtualTimeBudget) {
    uint64_t targetTime = m_time + virtualTimeBudget;
    int stepsThisFrame = 0;
    const int MAX_STEPS_PER_FRAME = 200000;

    while (!m_events.queue.empty() && m_events.queue.top().time <= targetTime) {
        if (++stepsThisFrame >= MAX_STEPS_PER_FRAME) break;

        auto event = m_events.queue.top();
        m_events.queue.pop();

        m_time = event.time;

        auto& comp = m_circuit.components[event.componentIndex];

        std::vector<PinState> inputs(comp.inputNets.size());

        for (size_t i = 0; i < comp.inputNets.size(); i++) {
            inputs[i] = m_circuit.netStates[comp.inputNets[i]];
        }

        std::vector<PinState> outputs(comp.outputNets.size());

        if (event.type == Event::Type::InputChanged) {
            comp.electronic->evaluate(this, inputs, outputs);
        } else {
            comp.electronic->onClockEdge(this, ClockEdge::Rising, inputs, outputs);
        }

        for (size_t i = 0; i < comp.outputNets.size(); i++) {
            driveNet(comp.outputNets[i], outputs[i], m_time);
        }
    }

    m_time = targetTime;
}

void CircuitRunner::notifyChange(IElectronic *electronic) {
    for (size_t i = 0; i < m_circuit.components.size(); i++) {
        if (electronic == m_circuit.components[i].electronic) {
            m_events.queue.push(Event{m_time, i, Event::Type::InputChanged});
        }
    }
}

size_t CircuitRunner::getComponentIndex(IElectronic *electronic) {
    for (size_t i = 0; i < m_circuit.components.size(); i++) {
        if (m_circuit.components[i].electronic == electronic) {
            return i;
        }
    }
    return -1;
}
