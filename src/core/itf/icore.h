//
// Created by henry on 9/9/26.
//

#ifndef CIRCUITSIM_ICORE_H
#define CIRCUITSIM_ICORE_H

#include <cstdint>
#include <vector>

struct CoreSnapshot {
    uint64_t cycleCount;
    uint64_t tickId;
    std::vector<uint8_t> state;
};

class ICore {
public:
    virtual ~ICore() = default;

    virtual uint64_t step(uint64_t cyclesRequested) = 0;
    virtual uint64_t writeSnapshot(CoreSnapshot& snapshot) const = 0;
    virtual double clockHz() const = 0;
};

#endif //CIRCUITSIM_ICORE_H
