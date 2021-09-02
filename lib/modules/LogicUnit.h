#pragma once

#include <vector>

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

struct LogicSlice_t {

    Gate_t output_wire;

    Gate_t z_bufs[6];

    Gate_t and_gate;
    Gate_t nand_gate;
    Gate_t or_gate;
    Gate_t nor_gate;
    Gate_t xor_gate;
    Gate_t xnor_gate;

    //
    // default ctor
    //
    LogicSlice_t(void);

    //
    // get the output wire
    //
    Gate_t get_output(void);

    //
    // set a input on all primitive gate types
    //
    void set_a(Gate_t a);

    //
    // set b input on all primitive gate types
    //
    void set_b(Gate_t b);

    //
    // static test method
    //
    static void Test(void);
};

struct LogicUnit_t {

    std::vector<LogicSlice_t> logic_slices;

    //
    // only ctor must specify size
    //
    LogicUnit_t(size_t width);

    //
    // set input a for specific slice
    //
    void set_a(size_t idx, Gate_t gate);

    //
    // set all a inputs
    //
    void set_a(std::vector<Gate_t> gates);

    //
    // set input a for specific slice
    //
    void set_b(size_t idx, Gate_t gate);

    //
    // set all b inputs
    //
    void set_b(std::vector<Gate_t> gates);

    //
    // get output from specific slice
    //
    Gate_t get_output(size_t idx);

    //
    // get vector of all output gates in logic slices
    //
    std::vector<Gate_t> get_output_vector(void);

    //
    // static test method
    //
    static void Test(void);
};

