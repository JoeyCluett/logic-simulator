#include <stdio.h>
#include <stdlib.h>

#include "logic-gates.h"
#include "exit_hook.h"

#ifdef __cplusplus
extern "C" {
#endif

logic_gate_t* ONE  = NULL;
logic_gate_t* ZERO = NULL;
logic_gate_t* HI_Z = NULL;
logic_gate_t* UND  = NULL;

void logic_verify_environment(void) {
    return;
}

void logic_gate_signal_set(logic_gate_t* g, int value) {
    if(g->gate_type == logic_gate_signal) {
        g->output_value = value;
    }
    else {
        exit_hook(stderr, 
                "logic_gate_signal_set : cannot manually set output value on non-logic_gate_signal type\n");
    }
}

void logic_gate_set(logic_gate_t* g, int value) {
    g->output_value = value;
}

logic_input_t* logic_input_new(void) {
    logic_input_t* inp_ptr = (logic_input_t*)malloc(sizeof(logic_input_t));
    return inp_ptr;
}

logic_gate_t* logic_gate_new(logic_type type) {
    logic_gate_t* gptr = (logic_gate_t*)malloc(sizeof(logic_gate_t));
    return logic_gate_init(gptr, type);
}

logic_gate_t* logic_gate_init(logic_gate_t* g, logic_type type) {
    
    g->gate_type = type;
    g->output_value = 0;

    switch(type) {
        case logic_gate_and : case logic_gate_nand:
        case logic_gate_or  : case logic_gate_nor :
        case logic_gate_xor : case logic_gate_xnor:
            g->gate.inputs   = NULL;
            g->gate.n_inputs = 0;
            g->gate.max_inputs = -1;
            break;
        case logic_gate_flipflop:
            g->flipflop.clk_input  = NULL;
            g->flipflop.data_input = NULL;
            g->flipflop.prev_clk_value = 0;
            break;
        case logic_gate_none:
        case logic_gate_constant:
        case logic_gate_signal:
            break; // do nothing
        case logic_gate_forward:
            //g->_forward.buffer_list = NULL; // NULL means list is empty
            g->_forward.input_gate  = NULL; // NULL means no input gate
            break;
        case logic_gate_buffer:
            g->tristate_buffer.data_input = NULL;
            g->tristate_buffer.select_input = NULL;
            break;
        case logic_gate_wire:
            g->wire.input_list = NULL;
            break;
        default:
            exit_hook(stderr,
                    "logic_gate_init : invalid logic gate type '%d'\n", (int)type);
    }

    // pointer really just gets passed through this function
    return g;
}

#ifdef __cplusplus
}
#endif
