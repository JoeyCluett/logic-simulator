#pragma once

#include <lib/modules/adders/CarryLookaheadUnit.h>
#include <lib/modules/adders/FullAdderCLA.h>

#include <vector>

struct CarryLookaheadAdder_t {
    
    CarryLookaheadUnit_t cla_unit;
    std::vector<FullAdder_CLA_t> adders;

    std::vector<Gate_t> A_forwards;
    std::vector<Gate_t> B_forwards;
    std::vector<Gate_t> Sum_forwards;
    Gate_t Cin_forward;
    Gate_t Cout_forward;

    size_t adder_width;

    //
    // only ctor must specify width of adder
    //
    CarryLookaheadAdder_t(size_t width);

    //
    // set A inputs
    //
    void set_a_inputs(std::vector<Gate_t> A);

    //
    // set B inputs
    //
    void set_b_inputs(std::vector<Gate_t> B);

    //
    // set carry in for adder
    //
    void set_carry_in(Gate_t);

    //
    // get all sum outputs
    //
    std::vector<Gate_t> get_sum(void);

    //
    // get carry out for adder
    //
    Gate_t get_carry_out(void);

    void VerifyInputs(void);

};
