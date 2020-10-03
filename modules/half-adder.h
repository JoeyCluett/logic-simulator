#pragma once

#include "../logic.h"

typedef struct half_adder_t {
    logic_gate_t* carry;
    logic_gate_t* sum;
} half_adder_t;


// simple half adder
half_adder_t* half_adder_new(void);
void half_adder_set_a(half_adder_t* ha, logic_gate_t* a);
void half_adder_set_b(half_adder_t* ha, logic_gate_t* b);
logic_gate_t* half_adder_get_carry(half_adder_t* ha);
logic_gate_t* half_adder_get_sum(half_adder_t* ha);

// ==========================================================================
// implmentation
// ==========================================================================

half_adder_t* half_adder_new(void) {
    half_adder_t* ha = (half_adder_t*)malloc(sizeof(half_adder_t));

    ha->sum   = XOR();
    ha->carry = AND();

    return ha;
}

void half_adder_set_a(half_adder_t* ha, logic_gate_t* a) {
    logic_gate_add_input(ha->sum,   a);
    logic_gate_add_input(ha->carry, a);
}

void half_adder_set_b(half_adder_t* ha, logic_gate_t* b) {
    logic_gate_add_input(ha->sum,   b);
    logic_gate_add_input(ha->carry, b);
}

logic_gate_t* half_adder_get_carry(half_adder_t* ha) {
    return ha->carry;
}

logic_gate_t* half_adder_get_sum(half_adder_t* ha) {
    return ha->sum;
}