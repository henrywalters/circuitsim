//
// Created by henry on 9/16/26.
//

#ifndef CIRCUITSIM_EVENTS_H
#define CIRCUITSIM_EVENTS_H
#include <cstdint>

struct Event {
    uint64_t time;
    std::size_t componentIndex;
    enum class Type {
        InputChanged,
        ClockTick,
    } type;
};

struct Events {
    struct EventCompare {
        bool operator()(Event const& a, Event const& b) const {
            return a.time > b.time;
        }
    };

    std::priority_queue<Event, std::vector<Event>, EventCompare> queue;
};

#endif //CIRCUITSIM_EVENTS_H
