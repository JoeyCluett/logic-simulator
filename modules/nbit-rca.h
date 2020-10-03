#pragma once

#include "full-adder.h"

// n-bit ripple carry

typedef struct rca_adder_t {
    full_adder_t* start_list;
    full_adder_t* end_list;
    int bitwidth;
} rca_adder_t;

rca_adder_t* rca_adder_new(int bitwidth);
rca_adder_t* rca_adder_init(rca_adder_t* adder, int bitwidth);
void rca_adder_set_a(rca_adder_t* adder, logic_gate_t* gate, int index);
void rca_adder_set_b(rca_adder_t* adder, logic_gate_t* gate, int index);
void rca_adder_set_ci(rca_adder_t* adder, logic_gate_t* gate);
logic_gate_t* rca_adder_get_sum(rca_adder_t* adder, int index);
logic_gate_t* rca_adder_get_carry(rca_adder_t* adder);

// ==========================================================================
// implmentation
// ==========================================================================

static void rca_verify_index(rca_adder_t* adder, int index) {
    if(index < 0 || index >= adder->bitwidth) {
        fprintf(stderr, "rca_verify_index : index '%d' is out of range\n", index);
        exit(1);
    }
}

logic_gate_t* rca_adder_get_carry(rca_adder_t* adder) {
    return adder->end_list->carry_out;
}

logic_gate_t* rca_adder_get_sum(rca_adder_t* adder, int index) {
    rca_verify_index(adder, index);
    return full_adder_get_sum(adder->start_list + index);
}

void rca_adder_set_ci(rca_adder_t* adder, logic_gate_t* gate) {
    full_adder_set_ci(adder->start_list, gate);
}

void rca_adder_set_a(rca_adder_t* adder, logic_gate_t* gate, int index) {
    rca_verify_index(adder, index);
    full_adder_set_a(adder->start_list + index, gate);
}

void rca_adder_set_b(rca_adder_t* adder, logic_gate_t* gate, int index) {
    rca_verify_index(adder, index);
    full_adder_set_b(adder->start_list + index, gate);
}

rca_adder_t* rca_adder_new(int bitwidth) {

    if(bitwidth < 1)
        fprintf(stderr, "rca_adder_new : invalid bitwidth of '%d'", bitwidth);

    // allocate full adders in single chunk
    full_adder_t* adderlist = (full_adder_t*)malloc(sizeof(full_adder_t) * bitwidth);

    // initialize all full adders in list
    int i = 0;
    for(; i < bitwidth; i++) full_adder_init(adderlist + i);

    // allocate structure to hold full adders
    rca_adder_t* adder = (rca_adder_t*)malloc(sizeof(rca_adder_t));

    adder->start_list = adderlist;
    adder->end_list   = adderlist + (bitwidth-1);
    adder->bitwidth   = bitwidth;

    // make carry-in/-out connections
    for(i = 1; i < bitwidth; i++)
        full_adder_set_ci(adderlist + i, (adderlist + (i - 1))->carry_out);

    return adder;
}
