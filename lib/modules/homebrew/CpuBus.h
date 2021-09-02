#pragma once

#include <vector>

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

struct CpuBus_t {

    Gate_t bus[16]; // wire

    //
    // ctor initializes bus as 'wire' type
    //
    CpuBus_t(void);

    //
    // connects given collection of gates to internal bus
    //
    void connect_to_bus(std::vector<Gate_t> interface);

    //
    // return bus interface
    // needed by many external modules
    //
    std::vector<Gate_t> get_bus(void);

};
