#ifndef OVERHEADQUEUE_HPP
#define OVERHEADQUEUE_HPP

#include "PacketQueue.hpp"

class OverheadQueue : public PacketQueue {
public:
    OverheadQueue();
    OverheadQueue(int count, int initVal, int extraVal);
    OverheadQueue(const OverheadQueue& other);
    virtual ~OverheadQueue();
    OverheadQueue& operator=(const OverheadQueue& other);

    virtual int totalBandwidth() const override;
};

#endif
