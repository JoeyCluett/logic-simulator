#include <stdio.h>
#include <string.h>

#include "logic-gates.h"
#include "logic-vector.h"
#include "logic-gate-utility.h"
#include "logic-gate-allocator.h"

static void logic_vector_verify_index(logic_vector_t* vec, int idx) {
    if(idx < 0 || idx >= vec->size) {
        fprintf(stderr, "logic_vector_verify_index : index '%d' is out of range\n", idx);
        exit(1);
    }
}

logic_vector_t* logic_vector_new(int sz, logic_type type) {
    logic_vector_t* vec = (logic_vector_t*)malloc(sizeof(logic_vector_t));
    return logic_vector_init(vec, sz, type);
}

logic_vector_t* logic_vector_init(logic_vector_t* vec, int sz, logic_type type) {
    vec->size     = sz;
    vec->type     = type;
    vec->gate_ptr = (logic_gate_t**)malloc(sizeof(logic_gate_t*) * sz);

    switch(type) {
        case logic_gate_and : case logic_gate_nand:
        case logic_gate_or  : case logic_gate_nor :
        case logic_gate_xor : case logic_gate_xnor:
            {
                int i;
                for(i = 0; i < sz; i++) {
                    logic_gate_t* g = logic_gate_alloc();

                    g->gate_type = type;
                    g->output_value = 0;

                    g->gate.inputs   = NULL;
                    g->gate.n_inputs = 0;

                    vec[i] = g;
                }
            }
            break;
        case logic_gate_flipflop:
            {
                int i;
                for(i = 0; i < sz; i++) {
                    logic_gate_t* g = logic_gate_alloc();

                    g->gate_type = type;
                    g->output_value = 0;
                    
                    g->flipflop.clk_input  = NULL;
                    g->flipflop.data_input = NULL;
                    g->flipflop.prev_clk_value = 0;

                    vec[i] = g;
                }
            }
            break;
        case logic_gate_none:
        case logic_gate_constant:
        case logic_gate_signal:
            break; // do nothing
        default:
            fprintf(stderr, "logic_gate_init : invalid logic gate type '%d'", (int)type);
            exit(1); // exit error
            break; // at this point, break from what lol ?!
    }

    int i;
    for(i = 0; i < sz; i++)
        vec->gate_ptr[i] = ;

    return vec;
}

logic_gate_t* logic_vector_get_bit_at(logic_vector_t* vec, int idx) {
    logic_vector_verify_index(vec, idx);
    return vec->gate_ptr[idx];
}

// set logic gate ptr at specific index. index is checked internally
void logic_vector_set_bit_at(logic_vector_t* vec, int index, int value) {

}

