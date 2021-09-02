#pragma once

#include <vector>
#include <lib/modules/adders/CarryLookaheadAdder.h>
#include <lib/modules/Multiplexer.h>

//
// converts two's complement to sign-magnitude format
//
struct TwosCmpl2SignMag_t {

    std::vector<Gate_t> input_inverters;
    std::vector<Gate_t> input_forwards;
    Gate_t sign_output;

    CarryLookaheadAdder_t adder;
    Multiplexer_t mux;

    TwosCmpl2SignMag_t(size_t input_width);

    void set_input(std::vector<Gate_t> v);
    auto get_output_magnitude(void) -> std::vector<Gate_t>;
    Gate_t get_output_sign(void);
};


