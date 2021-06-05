//
// contains definitions of wrapper functions and certain functions that 
// modify existing gate structures
//

#include "logic-gate-utility.h"
#include "logic-gates.h"
#include "logic-gate-allocator.h"

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// this is needed from logic-gate-allocator.c
extern logic_allocator_t global_logic_allocator;

// ==================================================================
// implementation
// ==================================================================

static void logic_gate_evaluate_forward_chain(logic_gate_t* d, logic_gate_t* s) {
    if(s->gate_type == logic_gate_forward) {
        // continue along the chain until a non-forward type is found
        logic_gate_evaluate_forward_chain(d, s->_forward.input_gate);
    }
    else {
        d->_forward.fetched_value = s->output_value;
    }
}

static int logic_gate_fetch_standard_gate(logic_gate_t* g) {

    switch(g->gate_type) {
        case logic_gate_and:
        case logic_gate_nand:
            g->gate.fetched_value = OUTPUT_ONE;
            logic_input_t* inp = g->gate.inputs;
            while(inp != NULL) {
                const int val = inp->output_ptr->output_value;
                if(val == OUTPUT_ZERO || val == OUTPUT_ONE) {
                    g->gate.fetched_value &= val;
                    inp = inp->next;
                }
                else {
                    g->gate.fetched_value = OUTPUT_UND;
                    inp = NULL;
                }
            }

            if(g->gate_type == logic_gate_nand) {
                switch(g->gate.fetched_value) {
                    case OUTPUT_ZERO: g->gate.fetched_value = OUTPUT_ONE; break;
                    case OUTPUT_ONE:  g->gate.fetched_value = OUTPUT_ZERO; break;
                    default: break;
                }
            }

            break;
        case logic_gate_or:  
        case logic_gate_nor:
            g->gate.fetched_value = OUTPUT_ZERO;
            logic_input_t* inp = g->gate.inputs;
            while(inp != NULL) {
                const int val = inp->output_ptr->output_value;
                if(val == OUTPUT_ZERO || val == OUTPUT_ONE) {
                    g->gate.fetched_value |= val;
                    inp = inp->next;
                }
                else {
                    g->gate.fetched_value = OUTPUT_UND;
                    inp = NULL;
                }
            }

            if(g->gate_type == logic_gate_nor) {
                switch(g->gate.fetched_value) {
                    case OUTPUT_ZERO: g->gate.fetched_value = OUTPUT_ONE; break;
                    case OUTPUT_ONE:  g->gate.fetched_value = OUTPUT_ZERO; break;
                    default: break;
                }
            }
            break;

        case logic_gate_xor:
        case logic_gate_xnor:
            g->gate.fetched_value = OUTPUT_ZERO;
            logic_input_t* inp = g->gate.inputs;
            while(inp != NULL) {
                const int val = inp->output_ptr->output_value;
                if(val == OUTPUT_ZERO || val == OUTPUT_ONE) {
                    g->gate.fetched_value ^= val;
                    inp = inp->next;
                }
                else {
                    g->gate.fetched_value = OUTPUT_UND;
                    inp = NULL;
                }
            }

            if(g->gate_type == logic_gate_xnor) {
                switch(g->gate.fetched_value) {
                    case OUTPUT_ZERO: g->gate.fetched_value = OUTPUT_ONE; break;
                    case OUTPUT_ONE:  g->gate.fetched_value = OUTPUT_ZERO; break;
                    default: break;
                }
            }
            break;

        default:
            fprintf(stderr, "logic_gate_fetch_standard_gate : unknown internal error -> invalid gate type %d\n", g->gate_type);
    }
}

static int logic_gate_update_standard_gate(logic_gate_t* g) {
    if(g->gate.fetched_value != g->output_value) {
        g->output_value = g->gate.fetched_value;
        return 1;
    }
    else {
        return 0;
    }
}

static int logic_gate_update_flipflop(logic_gate_t* g) {
    int new_output = g->output_value;
    
    if(g->flipflop.prev_clk_value == 0 &&
            g->flipflop.current_clk_value == 1) {
        // update only on the rising edge

        new_output = g->flipflop.data_input_value;
    }

    const int diff = (new_output != g->output_value);
    g->flipflop.prev_clk_value = g->flipflop.current_clk_value;
    g->output_value = new_output;
    return diff ? 1 : 0;
}

static void logic_gate_fetch_cycle(void) {

    logic_gate_chunk_t* chunk_ptr = global_logic_allocator.gate_first;
    int still_looping = 1;

    while(chunk_ptr != NULL && still_looping) {

        // iterate through current chunk of gates
        long int i = 0l;
        for(; i < chunk_ptr->n_allocated; i++) {

            //fprintf(stderr, "gate fetch cycle...\n");

            logic_gate_t* begin_ptr = chunk_ptr->gate_chunk + i;

            switch(begin_ptr->gate_type) {
                case logic_gate_and: 
                case logic_gate_nand:
                case logic_gate_or:  
                case logic_gate_nor:
                case logic_gate_xor: 
                case logic_gate_xnor:
                    logic_gate_fetch_standard_gate(begin_ptr);
                    break;

                case logic_gate_flipflop:
                    // just need to grab clk and data input values
                    begin_ptr->flipflop.current_clk_value = begin_ptr->flipflop.clk_input->output_value;
                    begin_ptr->flipflop.data_input_value  = begin_ptr->flipflop.data_input->output_value;
                    break;

                case logic_gate_none:
                    fprintf(stderr, "logic_gate_fetch_cycle : unknown internal error -> invalid fetch on NONE type gate");
                    break;

                case logic_gate_forward:
                    logic_gate_evaluate_forward_chain(
                            begin_ptr, 
                            begin_ptr->_forward.input_gate);
                    break;

                case logic_gate_buffer: // tri-state
                    begin_ptr->tristate_buffer.fetched_data   = begin_ptr->tristate_buffer.data_input->output_value;
                    begin_ptr->tristate_buffer.fetched_select = begin_ptr->tristate_buffer.select_input->output_value;
                    break;

                case logic_gate_bus: // passive wire type
                    {
                        begin_ptr->wire.fetched_data = OUTPUT_HI_Z;

                        logic_input_t* inp_ptr = begin_ptr->wire.input_list;
                        while(inp_ptr != NULL) {
                            
                            const val = inp_ptr->output_ptr->output_value;

                            switch(begin_ptr->wire.fetched_data) {
                                case OUTPUT_ZERO:
                                case OUTPUT_ONE:
                                    if(val != OUTPUT_HI_Z) {
                                        begin_ptr->wire.fetched_data = OUTPUT_UND;
                                        inp_ptr = NULL;
                                    }
                                    break;
                                case OUTPUT_HI_Z:
                                    begin_ptr->wire.fetched_data = val;
                                    break;
                                case OUTPUT_UND:
                                    inp_ptr = NULL;
                                    break;
                            }

                            inp_ptr = inp_ptr->next;
                        }
                    }
                    break;

                default:
                    fprintf(stderr, "logic_gate_fetch_cycle : unknown internal error -> invalid gate type in main simulation\n");
                    exit(1); break;
            }

            ++begin_ptr;
        }

        // advance to next chunk
        chunk_ptr = chunk_ptr->next;
    }
}


static int logic_gate_update_cycle(void) {

    logic_gate_chunk_t* chunk_ptr = global_logic_allocator.gate_first;
    int still_looping = 1;
    int changes = 0;

    while(chunk_ptr != NULL && still_looping) {

        // iterate through current chunk of gates
        long int i = 0;
        for(; i < chunk_ptr->n_allocated; i++) {

            // some pointer math
            logic_gate_t* begin_ptr = chunk_ptr->gate_chunk + i;

            switch(begin_ptr->gate_type) {
                case logic_gate_and:
                case logic_gate_nand:
                case logic_gate_or:
                case logic_gate_nor:
                case logic_gate_xor:
                case logic_gate_xnor:
                    changes += logic_gate_update_standard_gate(begin_ptr);
                    break;
                case logic_gate_flipflop:
                    logic_gate_update_flipflop(begin_ptr); 
                    break;
                case logic_gate_none:
                    fprintf(stderr, "logic_gate_update_cycle : unknown internal error -> cannot update NONE type gate\n");
                    exit(1); break;
                case logic_gate_forward:
                    break;
                case logic_gate_buffer: // tri-state
                    if(begin_ptr->output_value != begin_ptr->tristate_buffer.fetched_data) {
                        
                    }
                    else {

                    }
                    break;

                case logic_gate_bus: // passive wire type

                default:
                    fprintf(stderr, "logic_gate_update_cycle : unknown internal error -> invalid gate type in main simulation\n");
                    exit(1); break;
            }

            ++begin_ptr;
        }

        // advance to next chunk
        chunk_ptr = chunk_ptr->next;
    }

    return changes;
}

int logic_gate_simulate(void) {
    int ncycles    = 1;
    int sim_cycles = 0;

    while(ncycles) {
        logic_gate_fetch_cycle();
        ncycles = logic_gate_update_cycle();
        sim_cycles++;
    }

    return sim_cycles;
}

int logic_gate_simulate_callback(void(*callb)(void)) {
    int ncycles    = 1;
    int sim_cycles = 0;

    while(ncycles) {
        logic_gate_fetch_cycle();
        ncycles = logic_gate_update_cycle();
        sim_cycles++;
        callb();
    }

    return sim_cycles;
}

void logic_gate_flipflop_set_clock(logic_gate_t* g, logic_gate_t* clk) {
    if(g->gate_type == logic_gate_flipflop) {
        if(g->flipflop.clk_input == NULL) {

            // set clock input to gate ptr
            g->flipflop.clk_input = clk;

        }
        else {
            fprintf(stderr, "logic_gate_flipflop_set_clock : cannot set clock input -> previously set\n");
            exit(1);
        }
    }
    else {
        fprintf(stderr, "logic_gate_flipflop_set_clock : cannot set clock on non-logic_fate_flipflop type\n");
        exit(1);
    }
}

void logic_gate_flipflop_set_data(logic_gate_t* g, logic_gate_t* data) {
    if(g->gate_type == logic_gate_flipflop) {
        if(g->flipflop.data_input == NULL) {

            // set data input to gate ptr
            g->flipflop.data_input = data;

        }
        else {
            fprintf(stderr, "logic_gate_flipflop_set_data : cannot set clock input -> previously set\n");
            exit(1);
        }
    }
    else {
        fprintf(stderr, "logic_gate_flipflop_set_data : cannot set clock on non-logic_fate_flipflop type\n");
        exit(1);
    }
}

// adds gate input. input gate is pointer to another logic gate i.e. its output
// every combination needs to account for the presence of forwarding gates
void logic_gate_add_input(logic_gate_t* g, logic_gate_t* input) {

    if(g->gate_type == logic_gate_forward) {
        g->_forward.input_gate = input;
    }
    else {

        switch(g->gate_type) {
            case logic_gate_and:
            case logic_gate_nand:
            case logic_gate_or:
            case logic_gate_nor:
            case logic_gate_xor:
            case logic_gate_xnor:
                {
                    logic_input_t* inp_ptr = logic_input_alloc();
                    inp_ptr->output_ptr = input;
                    inp_ptr->next = g->gate.inputs;
                    g->gate.inputs = inp_ptr;
                }
                break;
            case logic_gate_bus:

        }

    }

}

logic_gate_t* SIGNAL(void) {
    logic_gate_t* ptr = logic_gate_new(logic_gate_signal);
    return ptr;
}

logic_gate_t* CONSTANT(int value) {

    switch(value) {
        case OUTPUT_ZERO: return ZERO;
        case OUTPUT_ONE:  return ONE;
        case OUTPUT_HI_Z: return HI_Z;
        case OUTPUT_UND:  return UND;
        default:
            fprintf(stderr, "CONSTANT : invalid constant value %d\n", value);
            exit(1);
    }
}

logic_gate_t* AND(void) {
    logic_gate_t* gptr = logic_gate_alloc();
    return logic_gate_init(gptr, logic_gate_and);
}

logic_gate_t* NAND(void) {
    logic_gate_t* gptr = logic_gate_alloc();
    return logic_gate_init(gptr, logic_gate_nand);
}

logic_gate_t* OR(void) {
    logic_gate_t* gptr = logic_gate_alloc();
    return logic_gate_init(gptr, logic_gate_or);
}

logic_gate_t* NOR(void) {
    logic_gate_t* gptr = logic_gate_alloc();
    return logic_gate_init(gptr, logic_gate_nor);
}

logic_gate_t* XOR(void) {
    logic_gate_t* gptr = logic_gate_alloc();
    return logic_gate_init(gptr, logic_gate_xor);
}

logic_gate_t* XNOR(void) {
    logic_gate_t* gptr = logic_gate_alloc();
    return logic_gate_init(gptr, logic_gate_xnor);
}

logic_gate_t* FLIPFLOP(void) {
    logic_gate_t* gptr = logic_gate_alloc();
    return logic_gate_init(gptr, logic_gate_flipflop);
}

logic_gate_t* FORWARD(void) {
    logic_gate_t* gptr = logic_gate_alloc();
    return logic_gate_init(gptr, logic_gate_forward);
}

logic_gate_t* WIRE(void) {
    logic_gate_t* gptr = logic_gate_alloc();
    return logic_gate_init(gptr, logic_gate_bus);
}

logic_gate_t* BUS(void) {
    return WIRE();
}

#ifdef __cplusplus
}
#endif

