#include <iostream>
#include "PacketQueue.hpp"
#include "OverheadQueue.hpp"

using namespace std;

// Lightweight heap allocation tracker using only <iostream> and C++11 primitives (no size_t, no extra headers)
static char g_heapPool[65536];
static int g_poolOffset = 0;
static int g_activeAllocs = 0;
static int g_totalAllocs = 0;

void* operator new(decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    // 8-byte alignment
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void* operator new[](decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void operator delete(void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete(void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

bool approxEqual(double a, double b) {
    double diff = a - b;
    if (diff < 0.0) diff = -diff;
    return diff < 0.001;
}

int main() {
    int passed = 0;
    int failed = 0;

    // Expected reference values computed dynamically
    // Array of 3 elements initialized as: initVal + 0, initVal + 1, initVal + 2
    int initV = static_cast<int>(64);
    int extV = static_cast<int>(20);

    // --- PARENT CLASS TEST CASES (1 to 5) ---
    // TC 1: Default constructor & empty compute
    {
        PacketQueue pDef;
        if (approxEqual(static_cast<double>(pDef.totalBandwidth()), 0.0) &&
            approxEqual(static_cast<double>(pDef.getPacketSize(0)), 0.0)) {
            cout << "[PASS] TC 01: PacketQueue default constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 01: PacketQueue default constructor" << endl;
            ++failed;
        }
    }

    // TC 2: Parameterized constructor & base compute()
    {
        PacketQueue p1(3, initV, extV);
        bool ok = approxEqual(static_cast<double>(p1.getPacketSize(0)), static_cast<double>(initV)) &&
                  approxEqual(static_cast<double>(p1.getPacketSize(2)), static_cast<double>(initV + static_cast<int>(2)));
        // Verify compute() is non-trivial when initV > 0
        if (ok && p1.getPacketSize(1) == static_cast<int>(initV + static_cast<int>(1))) {
            // Compare against a second identical instance
            PacketQueue pCheck(3, initV, extV);
            if (approxEqual(static_cast<double>(p1.totalBandwidth()), static_cast<double>(pCheck.totalBandwidth())) &&
                !approxEqual(static_cast<double>(p1.totalBandwidth()), 0.0)) {
                cout << "[PASS] TC 02: PacketQueue parameterized constructor & totalBandwidth()" << endl;
                ++passed;
            } else {
                cout << "[FAIL] TC 02: PacketQueue totalBandwidth() returned unexpected value" << endl;
                ++failed;
            }
        } else {
            cout << "[FAIL] TC 02: PacketQueue element initialization mismatch" << endl;
            ++failed;
        }
    }

    // TC 3: Parent Copy Constructor (Deep Copy verification)
    {
        PacketQueue orig(3, initV, extV);
        PacketQueue copyObj(orig);
        if (approxEqual(static_cast<double>(copyObj.totalBandwidth()), static_cast<double>(orig.totalBandwidth())) &&
            approxEqual(static_cast<double>(copyObj.getPacketSize(1)), static_cast<double>(orig.getPacketSize(1))) &&
            !approxEqual(static_cast<double>(copyObj.totalBandwidth()), 0.0)) {
            cout << "[PASS] TC 03: PacketQueue copy constructor deep copy" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 03: PacketQueue copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 4: Parent Copy Assignment Operator & Self-Assignment
    {
        PacketQueue a(3, initV, extV);
        PacketQueue b(2, initV, extV);
        b = a;
        PacketQueue* selfPtr = &b;
        b = *selfPtr; // Self-assignment check
        if (approxEqual(static_cast<double>(b.totalBandwidth()), static_cast<double>(a.totalBandwidth())) &&
            approxEqual(static_cast<double>(b.getPacketSize(2)), static_cast<double>(a.getPacketSize(2))) &&
            !approxEqual(static_cast<double>(b.totalBandwidth()), 0.0)) {
            cout << "[PASS] TC 04: PacketQueue copy assignment & self-assignment" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 04: PacketQueue copy assignment failed" << endl;
            ++failed;
        }
    }

    // TC 5: Parent Destructor Heap Release Check
    {
        int before = g_activeAllocs;
        {
            PacketQueue temp(4, initV, extV);
        }
        int after = g_activeAllocs;
        if (before == after && g_totalAllocs > 0) {
            cout << "[PASS] TC 05: PacketQueue destructor released heap memory" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 05: PacketQueue destructor leaked heap memory" << endl;
            ++failed;
        }
    }

    // --- CHILD CLASS TEST CASES (6 to 10) ---
    // TC 6: Child Parameterized Construction & Overridden compute()
    {
        PacketQueue baseObj(3, initV, extV);
        OverheadQueue childObj(3, initV, extV);
        if (!approxEqual(static_cast<double>(childObj.totalBandwidth()), 0.0) &&
            !approxEqual(static_cast<double>(childObj.totalBandwidth()), static_cast<double>(baseObj.totalBandwidth()))) {
            cout << "[PASS] TC 06: OverheadQueue overridden totalBandwidth()" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 06: OverheadQueue overridden totalBandwidth() failed" << endl;
            ++failed;
        }
    }

    // TC 7: Polymorphic Dispatch via Base Pointer
    {
        OverheadQueue directChild(3, initV, extV);
        PacketQueue* polyPtr = &directChild;
        if (!approxEqual(static_cast<double>(polyPtr->totalBandwidth()), 0.0) &&
            approxEqual(static_cast<double>(polyPtr->totalBandwidth()), static_cast<double>(directChild.totalBandwidth()))) {
            cout << "[PASS] TC 07: OverheadQueue polymorphic dispatch via PacketQueue*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 07: OverheadQueue polymorphic dispatch failed" << endl;
            ++failed;
        }
    }

    // TC 8: Child Copy Constructor
    {
        OverheadQueue c1(3, initV, extV);
        OverheadQueue c2(c1);
        if (approxEqual(static_cast<double>(c1.totalBandwidth()), static_cast<double>(c2.totalBandwidth())) &&
            !approxEqual(static_cast<double>(c2.totalBandwidth()), 0.0)) {
            cout << "[PASS] TC 08: OverheadQueue copy constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 08: OverheadQueue copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 9: Child Copy Assignment Operator
    {
        OverheadQueue c1(3, initV, extV);
        OverheadQueue c2;
        c2 = c1;
        if (approxEqual(static_cast<double>(c1.totalBandwidth()), static_cast<double>(c2.totalBandwidth())) &&
            !approxEqual(static_cast<double>(c2.totalBandwidth()), 0.0)) {
            cout << "[PASS] TC 09: OverheadQueue copy assignment operator" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 09: OverheadQueue copy assignment operator failed" << endl;
            ++failed;
        }
    }

    // TC 10: Virtual Destructor via Base Pointer Deletion
    {
        int before = g_activeAllocs;
        PacketQueue* heapChild = new OverheadQueue(4, initV, extV);
        delete heapChild;
        int after = g_activeAllocs;
        if (before == after) {
            cout << "[PASS] TC 10: OverheadQueue virtual destructor via PacketQueue*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 10: OverheadQueue virtual destructor leaked memory" << endl;
            ++failed;
        }
    }

    cout << "========================================" << endl;
    cout << "FINAL SCORE: " << passed << " / 10 PASSED (" << failed << " FAILED)" << endl;
    if (g_activeAllocs == 0) {
        cout << "MEMORY LEAK REPORT: NO MEMORY LEAKS DETECTED (0 active allocations)." << endl;
    } else {
        cout << "MEMORY LEAK REPORT: WARNING! " << g_activeAllocs << " active heap allocation(s) leaked!" << endl;
    }
    cout << "========================================" << endl;

    return (failed == 0 && g_activeAllocs == 0) ? 0 : 1;
}
