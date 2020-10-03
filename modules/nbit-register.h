#pragma once

#include "../logic.h"

typedef struct nbit_register_t {

    logic_gate_t** reg_ptr;
    int bitwidth;

} nbit_register_t;

nbit_register_t* nbit_register_new(int bitwidth);
nbit_register_t* nbit_register_init(nbit_register_t* reg, int bitwidth);
void nbit_register_set_clock(nbit_register_t* reg, logic_gate_t* clk);
void nbit_register_set_data(nbit_register_t* reg, logic_gate_t* data, int index);
logic_gate_t* nbit_register_get_q(nbit_register_t* reg, int idx);

// ==========================================================================
// implmentation
// ==========================================================================

static void nbit_register_verify_index(nbit_register_t* reg, int index) {
    if(index < 0 || index >= reg->bitwidth) {
        fprintf(stderr, "rca_verify_index : index '%d' is out of range\n", index);
        exit(1);
    }
}

nbit_register_t* nbit_register_new(int bitwidth) {
    nbit_register_t* reg = (nbit_register_t*)malloc(sizeof(nbit_register_t));
    return nbit_register_init(reg, bitwidth);
}

nbit_register_t* nbit_register_init(nbit_register_t* reg, int bitwidth) {

    if(bitwidth < 1) {
        fprintf(stderr, "nbit_register_init : invalid bit width '%d'\n", bitwidth);
        exit(1);
    }

    // due to the pre-allocation mechanism used by the simulator, its not really feasible to 
    // directly allocate 8 logic_gate_t structs wherever we want. so, instead, we allocate 8 
    // ptrs and initialize them with the addresses of pre-allocated gates
    logic_gate_t** regarray = (logic_gate_t**)malloc(sizeof(logic_gate_t*) * bitwidth);

    // initialize as flip-flop type. these are always D flip flops. other types of flip flops can be created with this one
    int i;
    for(i = 0; i < bitwidth; i++)
        regarray[i] = FLIPFLOP(); // assign value to pointer at this location

    reg->reg_ptr  = regarray;
    reg->bitwidth = bitwidth;

    return reg;
}

void nbit_register_set_clock(nbit_register_t* reg, logic_gate_t* clk) {
    int bitwidth = reg->bitwidth;

    int i;
    for(i = 0; i < bitwidth; i++)
        logic_gate_flipflop_set_clock(reg->reg_ptr[i], clk);
}

void nbit_register_set_data(nbit_register_t* reg, logic_gate_t* data, int index) {
    nbit_register_verify_index(reg, index);
    logic_gate_flipflop_set_data(reg->reg_ptr[index], data);
}

logic_gate_t* nbit_register_get_q(nbit_register_t* reg, int index) {
    nbit_register_verify_index(reg, index);
    return reg->reg_ptr[index];
}


