#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>
#include <lib/modules/BufferedFlipFlop.h>

#include <vector>

struct Register_t {

    std::vector<BufferedFlipFlop_t> flipflops;

    //
    // only ctor, must specify size
    //
    Register_t(size_t width);

    //
    // get specific flipflop output buffer
    //
    Gate_t get_output(int idx);

    //
    // get all flipflop output buffers
    //
    std::vector<Gate_t> get_output(void);

    //
    // sets data input for specific flipflop element
    //
    void set_data(int idx, Gate_t gate);

    //
    // set all data inputs
    //
    void set_data(std::vector<Gate_t> gates);

    //
    // sets clock for all flipflop elements
    //
    void set_clock(Gate_t gate);

    //
    // sets enable input for all tristate buffers
    //
    void set_enable(Gate_t gate);

};
