#pragma once

#include <vector>

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

struct AssertConstants_t {

    Gate_t buffer_one[16];
    Gate_t buffer_zero[16];
    Gate_t buffer_minus_one[16];    

    //
    // default ctor
    //
    AssertConstants_t(void);

    //
    // attach all the output buffers to given output bus
    //
    void attach_to_output_bus(std::vector<Gate_t> output_bus);

    //
    // set enable for 1 constant
    //
    void set_enable_one(Gate_t gate);

    //
    // set enable for -1 constant
    //
    void set_enable_minus_one(Gate_t gate);

    //
    // set enable for 0 constant
    //
    void set_enable_zero(Gate_t gate);

};

