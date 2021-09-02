#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

struct BufferedFlipFlop_t {

    Gate_t flipflop;
    Gate_t buffer;

    //
    // default ctor
    //
    BufferedFlipFlop_t(void);

    //
    // returns buffer output
    //
    Gate_t get_output(void);

    //
    // set data input for flipflop
    //
    void set_data_input(Gate_t gate);

    //
    // set clock input for flipflop
    //
    void set_clock_input(Gate_t gate);

    //
    // set enable input for tristate buffer
    //
    void set_enable_input(Gate_t gate);

};

