#pragma once

#include <stdio.h>
#include "../logic.h"
#include "nbit-register.h"
#include "nbit-rca.h"

typedef struct nbit_counter_t {

    nbit_register_t reg;
    rca_adder_t adder;
    logic_gate_t** and_gates;
    int bitwidth;

} nbit_counter_t;

nbit_counter_t* nbit_counter_new(int bitwidth);
nbit_counter_t* nbit_counter_init(nbit_counter_t* counter, int bitwidth);
void nbit_counter_set_clock(nbit_counter_t* counter, logic_gate_t* clk);
void nbit_counter_set_nreset(nbit_counter_t* counter, logic_gate_t* nreset);
logic_gate_t* nbit_counter_get_q(nbit_counter_t* counter, int index);

// ==========================================================================
// implmentation
// ==========================================================================

nbit_counter_t* nbit_counter_init(nbit_counter_t* counter, int bitwidth) {
    
    if(bitwidth < 1) {
        fprintf(stderr, "nbit_counter_init : invalid bit width '%d'\n", bitwidth);
        exit(1);
    }

    counter->bitwidth = bitwidth;

    // initialize the submodules
    nbit_register_init(&counter->reg, bitwidth);
    rca_adder_init(&counter->adder, bitwidth);

    // allocate pointers to and gates
    counter->and_gates = (logic_gate_t**)malloc(sizeof(logic_gate_t*) * bitwidth); //new logic_gate_t*[bitwidth];

    int i;
    for(i = 0; i < bitwidth; i++) {
        counter->and_gates[i] = AND();
        rca_adder_set_a(&counter->adder, counter->and_gates[i], i);
    
        

        // always want to increment by one
        if(i == 0) rca_adder_set_b(&counter->adder, ONE,  0);
        else       rca_adder_set_b(&counter->adder, ZERO, i);
    }

    return counter;
}

