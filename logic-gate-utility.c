// contains definitions of wrapper functions and certain functions that 
// modify existing gate structures

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

static int logic_gate_update_and(logic_gate_t* g) {

    //fprintf(stderr, "and gate -> n high : %d, n inputs : %d\n", g->gate.n_high, g->gate.n_inputs);

    int new_output = 0;
    if(g->gate.n_inputs == g->gate.n_high)
        new_output = 1;
    int diff = (new_output != g->output_value);
    g->output_value = new_output;
    return diff ? 1 : 0;
}

static int logic_gate_update_nand(logic_gate_t* g) {
    int new_output = 1;
    if(g->gate.n_inputs == g->gate.n_high)
        new_output = 0;
    int diff = (new_output != g->output_value);
    g->output_value = new_output;
    return diff ? 1 : 0;
}

static int logic_gate_update_or(logic_gate_t* g) {
    int new_output = 0;
    if(g->gate.n_high > 0)
        new_output = 1;
    int diff = (new_output != g->output_value);
    g->output_value = new_output;
    return diff ? 1 : 0;
}

static int logic_gate_update_nor(logic_gate_t* g) {
    int new_output = 1;
    if(g->gate.n_high > 0)
        new_output = 0;
    int diff = (new_output != g->output_value);
    g->output_value = new_output;
    return diff ? 1 : 0;
}

static int logic_gate_update_xor(logic_gate_t* g) {
    int new_output = g->gate.n_high & 1;
    int diff = (new_output != g->output_value);
    g->output_value = new_output;
    return diff ? 1 : 0;
}

static int logic_gate_update_xnor(logic_gate_t* g) {
    int new_output = 1 - (g->gate.n_high & 1);
    int diff = (new_output != g->output_value);
    g->output_value = new_output;
    return diff ? 1 : 0;
}

static int logic_gate_update_flipflop(logic_gate_t* g) {
    int new_output = g->output_value;
    
    if(g->flipflop.prev_clk_value == 0 &&
            g->flipflop.current_clk_value == 1) {
        // update only on the rising edge

        new_output = g->flipflop.data_input_value;
    }
    int diff = (new_output != g->output_value);
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
                case logic_gate_and: case logic_gate_nand:
                case logic_gate_or:  case logic_gate_nor:
                case logic_gate_xor: case logic_gate_xnor:
                    // all basic gate types go through this procedure

                    // reset previous fetch data
                    begin_ptr->gate.n_high = 0;
                    begin_ptr->gate.n_low  = 0;
                    {
                        // iterate through inputs and count high/low occurances
                        logic_input_t* in = begin_ptr->gate.inputs;
                        while(in != NULL) {

                            switch(in->output_ptr->output_value) {
                                case 0: begin_ptr->gate.n_low++;  break;
                                case 1: begin_ptr->gate.n_high++; break;
                                default:
                                    fprintf(stderr, 
                                            "logic_gate_fetch_cycle : gate has invalid output value '%d'\n", 
                                            in->output_ptr->output_value);
                                    exit(1); break;
                            }

                            in = in->next;
                        }
                    }
                    break;

                case logic_gate_flipflop:
                    // just need to grab clk and data input values
                    begin_ptr->flipflop.current_clk_value = begin_ptr->flipflop.clk_input->output_value;
                    begin_ptr->flipflop.data_input_value  = begin_ptr->flipflop.data_input->output_value;
                    break;

                case logic_gate_none:
                case logic_gate_forward:
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
                case logic_gate_and:  changes += logic_gate_update_and(begin_ptr);  break;
                case logic_gate_nand: changes += logic_gate_update_nand(begin_ptr); break;
                case logic_gate_or:   changes += logic_gate_update_or(begin_ptr);   break;
                case logic_gate_nor:  changes += logic_gate_update_nor(begin_ptr);  break;
                case logic_gate_xor:  changes += logic_gate_update_xor(begin_ptr);  break;
                case logic_gate_xnor: changes += logic_gate_update_xnor(begin_ptr); break;
                case logic_gate_flipflop:
                    logic_gate_update_flipflop(begin_ptr); 
                    break;
                case logic_gate_none:
                case logic_gate_forward:
                    break;
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

    if(g->gate_type == logic_gate_forward && input->gate_type == logic_gate_forward) {
        // chaining forward gates is tricky
        if(input->_forward.next_forward_gate == NULL) {
            input->_forward.next_forward_gate = g;
        }
        else {
            fprintf(stderr, "logic_gate_add_input : cannot chain more than one forward gate to same destination forward gate");
            exit(1);
        }
    }
    else if(g->gate_type == logic_gate_forward && input->gate_type != logic_gate_forward) {

        // cannot forward more than one input gate
        if(g->_forward.input_gate != NULL) {
            fprintf(stderr, "logic_gate_add_input : cannot set more than one input for forward gate");
            exit(1);
        }

        // keep this reference in case additional gates ever need to be forwarded to
        g->_forward.input_gate = input;

        // if we have gates, iterate through and attach this input to them
        if(g->_forward.buffer_list != NULL) {
            logic_input_t* iter = g->_forward.buffer_list;

            while(iter) {
                logic_gate_add_input(iter->output_ptr, input);
                iter = iter->next;
            }

            // now that we have forwarded, we can delete our references to downstream gates
            iter = g->_forward.buffer_list;
            while(iter) {
                logic_input_t* next_ptr = iter->next;
                free(iter);
                iter = next_ptr;
            }
            g->_forward.buffer_list = NULL;
        }

        // if there is a chained forward gate ahead, forward input to it
        if(g->_forward.next_forward_gate != NULL) {
            logic_gate_add_input(g->_forward.next_forward_gate, input);
        }
    }
    else if(g->gate_type != logic_gate_forward && input->gate_type == logic_gate_forward) {
        
        if(input->_forward.input_gate == NULL) {
            // input hasnt been set yet. THATS OK!

            logic_input_t* logic_input_ptr = logic_input_new();
            logic_input_ptr->next = input->_forward.buffer_list;
            logic_input_ptr->output_ptr = g; // buffer for later reference

            input->_forward.buffer_list = logic_input_ptr;

        }
        else {
            // input has been set, just forward it to this gate now
            logic_gate_add_input(g, input->_forward.input_gate);
        }

    }
    else {
        // cant add inputs to flipflops, none types, constants or signals
        if(g->gate_type != logic_gate_flipflop &&
                g->gate_type != logic_gate_none &&
                g->gate_type != logic_gate_constant &&
                g->gate_type != logic_gate_signal) {

                // allocate new logic_input_t struct
                logic_input_t* new_input = logic_input_alloc();
                new_input->output_ptr = input;

                // place new input at beginning of list of inputs
                new_input->next = g->gate.inputs;
                g->gate.inputs = new_input;
                g->gate.n_inputs++;
        }
        else {
            fprintf(stderr, "logic_gate_add_input : cannot add input to logic_gate_flipflop or logic_gate_none type\n");
            exit(1);
        }
    }
}

logic_gate_t* SIGNAL(void) {
    logic_gate_t* ptr = logic_gate_new(logic_gate_signal);
    return ptr;
}

logic_gate_t* CONSTANT(int value) {
    logic_gate_t* ptr = NULL;
    
    if(value == 0)      ptr = ZERO;
    else if(value == 1) ptr = ONE;
    else {
        ptr = logic_gate_new(logic_gate_constant);
        ptr->output_value = value;
    }

    return ptr;
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

#ifdef __cplusplus
}
#endif

