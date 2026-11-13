#ifndef PACKETQUEUE_HPP
#define PACKETQUEUE_HPP

#include <iostream>
using namespace std;

class PacketQueue {
protected:
    int* byteSizes;
    int packetCount;
    int headerBytes;

public:
    PacketQueue();
    PacketQueue(int count, int initVal, int extraVal);
    PacketQueue(const PacketQueue& other);
    virtual ~PacketQueue();
    PacketQueue& operator=(const PacketQueue& other);

    int getPacketSize(int index) const;
    virtual int totalBandwidth() const;
};

#endif
