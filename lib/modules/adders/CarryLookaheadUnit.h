#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>
#include <lib/cpp-api/gate-varargs.h>

#include <vector>

//
// algorithmically generates N-bit carry lookahead unit
//
struct CarryLookaheadUnit_t {

    std::vector<Gate_t> p;
    std::vector<Gate_t> g;
    std::vector<Gate_t> c;
    size_t cla_width;

    //Gate_t PG;
    //Gate_t GG;

    //
    // ctor must specify width
    //
    CarryLookaheadUnit_t(size_t width);

    //
    // set generate inputs
    //
    void set_generate_inputs(std::vector<Gate_t> g);

    //
    // set c0 input
    //
    void set_cin_input(Gate_t g);

    //
    // set propagate inputs
    //
    void set_propagate_inputs(std::vector<Gate_t> p);

    //
    // return carry outputs
    //
    std::vector<Gate_t> get_carry_outputs(void);

/*
    //
    // get group propagate
    //
    Gate_t get_propagate(void);

    //
    // get group generate
    //
    Gate_t get_generate(void);
*/

    void VerifyInputs(void);

};


