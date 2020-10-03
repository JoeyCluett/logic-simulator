#pragma once

#include "../logic.h"
#include "half-adder.h"

// full adder composed of two half adders
typedef struct full_adder_t {
    half_adder_t* upper;
    half_adder_t* lower;
    logic_gate_t* carry_out;
} full_adder_t;


full_adder_t* full_adder_new(void);
full_adder_t* full_adder_init(full_adder_t* fa);
void full_adder_set_a(full_adder_t* fa, logic_gate_t* a);
void full_adder_set_b(full_adder_t* fa, logic_gate_t* b);
void full_adder_set_ci(full_adder_t* fa, logic_gate_t* ci);
logic_gate_t* full_adder_get_carry(full_adder_t* fa);
logic_gate_t* full_adder_get_sum(full_adder_t* fa);

// ==========================================================================
// implmentation
// ==========================================================================

logic_gate_t* full_adder_get_carry(full_adder_t* fa) {
    return half_adder_get_carry(fa->upper);
}

logic_gate_t* full_adder_get_sum(full_adder_t* fa) {
    return half_adder_get_sum(fa->upper);
}

void full_adder_set_ci(full_adder_t* fa, logic_gate_t* ci) {
    half_adder_set_b(fa->upper, ci);
}

void full_adder_set_a(full_adder_t* fa, logic_gate_t* a) {
    half_adder_set_a(fa->lower, a);
}

void full_adder_set_b(full_adder_t* fa, logic_gate_t* b) {
    half_adder_set_b(fa->lower, b);
}

full_adder_t* full_adder_new(void) {
    full_adder_t* fa = (full_adder_t*)malloc(sizeof(full_adder_t));
    return full_adder_init(fa);
}

full_adder_t* full_adder_init(full_adder_t* fa) {

    // build up full adder
    fa->upper = half_adder_new();
    fa->lower = half_adder_new();
    fa->carry_out = OR();

    // specify connections between half adders
    half_adder_set_a(fa->upper, fa->lower->sum);

    // route signals to carry out OR gate
    logic_gate_add_input(fa->carry_out, fa->upper->carry);
    logic_gate_add_input(fa->carry_out, fa->lower->carry);

    return fa;
}



