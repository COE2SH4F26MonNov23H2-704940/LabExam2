#include "OverheadQueue.hpp"

OverheadQueue::OverheadQueue() : PacketQueue() {}

OverheadQueue::OverheadQueue(int count, int initVal, int extraVal) : PacketQueue(count, initVal, extraVal) {}

OverheadQueue::OverheadQueue(const OverheadQueue& other) : PacketQueue(other) {}

OverheadQueue::~OverheadQueue() {}

OverheadQueue& OverheadQueue::operator=(const OverheadQueue& other) {
    if (this != &other) {
        PacketQueue::operator=(other);
    }
    return *this;
}

// TODO 5: Override totalBandwidth() (Maximum 8 lines of implementation)
int OverheadQueue::totalBandwidth() const {
    return static_cast<int>(0);
}
