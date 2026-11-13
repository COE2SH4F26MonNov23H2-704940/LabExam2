#include "PacketQueue.hpp"

PacketQueue::PacketQueue() : byteSizes(nullptr), packetCount(0), headerBytes(20) {}

PacketQueue::PacketQueue(int count, int initVal, int extraVal) : byteSizes(nullptr), packetCount(count), headerBytes(extraVal) {
    if (packetCount > 0) {
        byteSizes = new int[packetCount];
        for (int i = 0; i < packetCount; ++i) {
            byteSizes[i] = initVal + static_cast<int>(i);
        }
    } else {
        packetCount = 0;
    }
}

// TODO 1: Complete the Copy Constructor (Deep Copy)
PacketQueue::PacketQueue(const PacketQueue& other) {
    (void)other;
}

// TODO 2: Complete the Destructor (Release DMA)
PacketQueue::~PacketQueue() {
}

// TODO 3: Complete the Copy Assignment Operator (Deep Copy + Self-Assignment Guard)
PacketQueue& PacketQueue::operator=(const PacketQueue& other) {
    (void)other;
    return *this;
}

int PacketQueue::getPacketSize(int index) const {
    if (index >= 0 && index < packetCount && byteSizes != nullptr) {
        return byteSizes[index];
    }
    return static_cast<int>(0);
}

// TODO 4: Implement totalBandwidth() (Maximum 8 lines of implementation)
int PacketQueue::totalBandwidth() const {
    return static_cast<int>(0);
}
