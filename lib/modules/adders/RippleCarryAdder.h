#pragma once

#include <vector>

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>
#include <lib/modules/adders/FullAdder.h>

struct RippleCarryAdder_t {

    std::vector<FullAdder_t> adders;

    //
    // only ctor, must specify width for RCA
    //
    RippleCarryAdder_t(size_t width);

    //
    // set a input for specific adder
    //
    void set_a(int idx, Gate_t inp);
    
    //
    // set all a inputs
    //
    void set_a(std::vector<Gate_t> inp);

    //
    // set b for specific adder
    //
    void set_b(int idx, Gate_t inp);

    //
    // set all b inputs
    //
    void set_b(std::vector<Gate_t> inp);

    //
    // set carry-in for entire RCA
    //
    void set_ci(Gate_t inp);

    //
    // get sum output from specific adder
    //
    Gate_t get_sum(int idx);

    //
    // set vec of all sum outputs
    //
    std::vector<Gate_t> get_sum(void);

    //
    // get carry-out for entire RCA
    //
    Gate_t get_co(void);

};

