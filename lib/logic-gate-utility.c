//
// contains definitions of wrapper functions and certain functions that 
// modify existing gate structures
//

#include <lib/logic-gate-utility.h>
#include <lib/logic-gates.h>
#include <lib/logic-gate-allocator.h>
#include <lib/exit_hook.h>

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>

// this is needed from logic-gate-allocator.c
extern logic_allocator_t global_logic_allocator;

static int logic_gate_fetch_standard_gate(logic_gate_t* g);
static int logic_gate_update_standard_gate(logic_gate_t* g);
static int logic_gate_update_flipflop(logic_gate_t* g);
static inline int logic_gate_get_output_value(logic_gate_t* g);
static void logic_gate_fetch_cycle(void);
static int logic_gate_update_cycle(void);
static const char* logic_gate_type_as_string(logic_gate_t* g);

// ==================================================================
// implementation
// ==================================================================

logic_gate_t* logic_gate_follow(logic_gate_t* g) {
    while(
            g != NULL && 
            g->gate_type == logic_gate_forward) {

        g = g->_forward.input_gate;
    }

    return g;
}

void logic_gate_eval_forwards(void) {
    logic_gate_chunk_t* chunk_ptr = global_logic_allocator.gate_first;

    while(chunk_ptr != NULL) {

        long int i = 0l;
        for(; i < chunk_ptr->n_allocated; i++) {

            logic_gate_t* gate_ptr = chunk_ptr->gate_chunk + i;

            switch(gate_ptr->gate_type) {
                case logic_gate_and     ://  = 0,
                case logic_gate_nand    ://  = 1,
                case logic_gate_or      ://  = 2,
                case logic_gate_nor     ://  = 3,
                case logic_gate_xor     ://  = 4,
                case logic_gate_xnor    ://  = 5,
                    {
                        logic_input_t* inp = gate_ptr->gate.inputs;
                        while(inp != NULL) {
                            logic_gate_t* tmp_gate_ptr = logic_gate_follow(inp->output_ptr);
                            inp->output_ptr = tmp_gate_ptr;    
                            inp = inp->next;
                        }
                    }
                    break;

                case logic_gate_flipflop://  = 6,
                    {
                        logic_gate_t* gptr = logic_gate_follow(gate_ptr->flipflop.clk_input);
                        gate_ptr->flipflop.clk_input = gptr;

                        gptr = logic_gate_follow(gate_ptr->flipflop.data_input);
                        gate_ptr->flipflop.data_input = gptr;
                    }
                    break;

                case logic_gate_none    ://  = 7,  // for pre-allocated gate types
                    exit_hook(stderr, 
                            "logic_gate_eval_forwards : cannot get output of 'none' type\n");
                    
                case logic_gate_constant://  = 8,
                case logic_gate_signal  ://  = 9,  // input type
                case logic_gate_forward ://  = 10, // dont bother tracing certain types
                    break;

                case logic_gate_buffer  ://  = 11, // tri-state buffer
                    {
                        logic_gate_t* gptr = logic_gate_follow(gate_ptr->tristate_buffer.data_input);
                        gate_ptr->tristate_buffer.data_input = gptr;

                        gptr = logic_gate_follow(gate_ptr->tristate_buffer.select_input);
                        gate_ptr->tristate_buffer.select_input = gptr;
                    }
                    break; 

                case logic_gate_wire    ://  = 12, // passive wire type
                    {
                        logic_input_t* inp = gate_ptr->wire.input_list;
                        while(inp != NULL) {
                            logic_gate_t* tmp_gate_ptr = logic_gate_follow(inp->output_ptr);
                            inp->output_ptr = tmp_gate_ptr;
                            inp = inp->next;
                        }
                    }
                    break;

                default:
                    exit_hook(stderr, 
                            "logic_gate_get_output_value : unknown internal error\n");
            }

        }

        chunk_ptr = chunk_ptr->next;
    }

}

static int logic_gate_fetch_standard_gate(logic_gate_t* g) {

    switch(g->gate_type) {
        case logic_gate_and:
        case logic_gate_nand:
            {
                g->gate.fetched_value = OUTPUT_ONE;
                logic_input_t* inp = g->gate.inputs;
                while(inp != NULL) {
                    const int val = logic_gate_get_output_value(inp->output_ptr);
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

            }

            break;

        case logic_gate_or:  
        case logic_gate_nor:
            {
                g->gate.fetched_value = OUTPUT_ZERO;
                logic_input_t* inp = g->gate.inputs;
                while(inp != NULL) {
                    const int val = logic_gate_get_output_value(inp->output_ptr);
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
            }
            break;

        case logic_gate_xor:
        case logic_gate_xnor:
            {
                g->gate.fetched_value = OUTPUT_ZERO;
                logic_input_t* inp = g->gate.inputs;
                while(inp != NULL) {
                    const int val = logic_gate_get_output_value(inp->output_ptr);
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
            }
            break;

        default:
            exit_hook(stderr, 
                    "logic_gate_fetch_standard_gate : unknown internal "
                    "error -> invalid gate type '%s'\n", 
                    logic_gate_type_as_string(g));
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

static inline int logic_gate_get_output_value(logic_gate_t* g) {

    switch(g->gate_type) {
        case logic_gate_and     ://  = 0,
        case logic_gate_nand    ://  = 1,
        case logic_gate_or      ://  = 2,
        case logic_gate_nor     ://  = 3,
        case logic_gate_xor     ://  = 4,
        case logic_gate_xnor    ://  = 5,
        case logic_gate_flipflop://  = 6,
            return g->output_value;

        case logic_gate_none    ://  = 7,  // for pre-allocated gate types
            exit_hook(stderr, 
                    "logic_gate_get_output_value : cannot get output of 'none' type\n");
            
        case logic_gate_constant://  = 8,
        case logic_gate_signal  ://  = 9,  // input type
            return g->output_value;

        case logic_gate_forward ://  = 10, // used as an optimization technique
            {
                logic_gate_t* chain_ptr = g->_forward.input_gate;
                while(chain_ptr->gate_type == logic_gate_forward)
                    chain_ptr = chain_ptr->_forward.input_gate;

                return logic_gate_get_output_value(chain_ptr);
            }

        case logic_gate_buffer  ://  = 11, // tri-state buffer
        case logic_gate_wire    ://  = 12, // passive wire type
            return g->output_value;

        default:
            exit_hook(stderr, 
                    "logic_gate_get_output_value : unknown internal error\n");
    }

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
                    begin_ptr->flipflop.current_clk_value = logic_gate_follow(begin_ptr->flipflop.clk_input)->output_value;
                    begin_ptr->flipflop.data_input_value  = logic_gate_follow(begin_ptr->flipflop.data_input)->output_value;
                    break;

                case logic_gate_none:
                    exit_hook(stderr, 
                            "logic_gate_fetch_cycle : unknown internal error -> invalid fetch on NONE type gate\n");

                case logic_gate_forward:
                    break; // forwards are never evaluated by themselves, only 
                           // when other gates need their output values

                case logic_gate_buffer: // tri-state
                    begin_ptr->tristate_buffer.fetched_data   = logic_gate_follow(begin_ptr->tristate_buffer.data_input)->output_value;
                    begin_ptr->tristate_buffer.fetched_select = logic_gate_follow(begin_ptr->tristate_buffer.select_input)->output_value;
                    break;

                case logic_gate_wire: // passive wire type
                    {
                        begin_ptr->wire.fetched_data = OUTPUT_HI_Z;

                        logic_input_t* inp_ptr = begin_ptr->wire.input_list;
                        while(inp_ptr != NULL) {
                            
                            logic_gate_t* inp_gate = logic_gate_follow(inp_ptr->output_ptr);

                            switch(inp_gate->output_value) {
                                case OUTPUT_ZERO:
                                    if(begin_ptr->wire.fetched_data == OUTPUT_HI_Z) {
                                        begin_ptr->wire.fetched_data = OUTPUT_ZERO;
                                    }
                                    else {
                                        inp_ptr = NULL;
                                    }
                                    break;
                                case OUTPUT_ONE:
                                    if(begin_ptr->wire.fetched_data == OUTPUT_HI_Z) {
                                        begin_ptr->wire.fetched_data = OUTPUT_ONE;
                                    }
                                    else {
                                        inp_ptr = NULL;
                                    }
                                    break;
                                case OUTPUT_HI_Z:
                                    break;
                                case OUTPUT_UND:
                                    begin_ptr->wire.fetched_data = OUTPUT_UND;
                                    break;
                            }

                            if(inp_ptr)
                                inp_ptr = inp_ptr->next;
                        }

                    }
                    break;

                default:
                    exit_hook(stderr, 
                            "logic_gate_fetch_cycle : unknown internal error -> invalid gate type in main simulation\n");
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
                    changes += logic_gate_update_flipflop(begin_ptr); 
                    break;

                case logic_gate_none:
                    exit_hook(stderr, 
                            "logic_gate_update_cycle : unknown internal error -> cannot update NONE type gate\n");

                case logic_gate_forward:
                    break;

                case logic_gate_buffer: // tri-state
                    if(begin_ptr->tristate_buffer.fetched_select == OUTPUT_ZERO) {
                        if(begin_ptr->output_value != OUTPUT_HI_Z) {
                            begin_ptr->output_value = OUTPUT_HI_Z;
                            changes++;
                        }
                    }
                    else {
                        if(begin_ptr->output_value != begin_ptr->tristate_buffer.fetched_data) {
                            begin_ptr->output_value = begin_ptr->tristate_buffer.fetched_data;
                            changes++;
                        }
                    }
                    break;

                case logic_gate_wire: // passive wire type
                    if(begin_ptr->output_value != begin_ptr->wire.fetched_data) {
                        begin_ptr->output_value = begin_ptr->wire.fetched_data;
                        changes++;
                    }
                    break;

                default:
                    exit_hook(stderr, 
                            "logic_gate_update_cycle : unknown internal error -> invalid gate type in main simulation\n");
            }

            ++begin_ptr;
        }

        // advance to next chunk
        chunk_ptr = chunk_ptr->next;
    }

    return changes;
}

static const char* logic_gate_type_as_string(logic_gate_t* g) {
    switch(g->gate_type) {
        case logic_gate_and:      return "and";
        case logic_gate_nand:     return "nand";
        case logic_gate_or:       return "or";
        case logic_gate_nor:      return "nor";
        case logic_gate_xor:      return "xor";
        case logic_gate_xnor:     return "xnor";
        case logic_gate_flipflop: return "flipflop";
        case logic_gate_none:     return "none";
        case logic_gate_constant: return "constant";
        case logic_gate_signal:   return "signal";
        case logic_gate_forward:  return "forward";
        case logic_gate_buffer:   return "buffer";
        case logic_gate_wire:     return "wire";
        default:
            exit_hook(stderr, 
                    "logic_gate_type_as_string : unknown internal error\n");
    }
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
    assert(g->gate_type == logic_gate_flipflop);
    assert(g->flipflop.clk_input == NULL);
    g->flipflop.clk_input = clk;
}

void logic_gate_flipflop_set_data(logic_gate_t* g, logic_gate_t* data) {
    assert(g->gate_type == logic_gate_flipflop);
    assert(g->flipflop.data_input == NULL);
    g->flipflop.data_input = data;
}

void logic_gate_tristate_set_data(logic_gate_t* g, logic_gate_t* data) {
    assert(g->gate_type == logic_gate_buffer);
    assert(g->tristate_buffer.data_input == NULL);
    g->tristate_buffer.data_input = data;
}

void logic_gate_tristate_set_enable(logic_gate_t* g, logic_gate_t* en) {
    assert(g->gate_type == logic_gate_buffer);
    assert(g->tristate_buffer.select_input == NULL);
    g->tristate_buffer.select_input = en;
}

// adds gate input. input gate is pointer to another logic gate i.e. its output
// every combination needs to account for the presence of forwarding gates
void logic_gate_add_input(logic_gate_t* g, logic_gate_t* input) {


    switch(g->gate_type) {
        case logic_gate_and:
        case logic_gate_nand:
        case logic_gate_or:
        case logic_gate_nor:
        case logic_gate_xor:
        case logic_gate_xnor:
            {

                if(g->gate.inputs == NULL) {
                    logic_input_t* inp_ptr = logic_input_alloc();
                    inp_ptr->output_ptr = input;
                    inp_ptr->next = g->gate.inputs;
                    g->gate.inputs = inp_ptr;
                }
                else {
                    logic_input_t* inp_ptr = g->gate.inputs;

                    // track to end of input linked list
                    while(inp_ptr->next != NULL)
                        inp_ptr = inp_ptr->next;

                    inp_ptr->next = logic_input_alloc();
                    inp_ptr->next->next = NULL;
                    inp_ptr->next->output_ptr = input;
                }

                // increment input counter
                g->gate.n_inputs++;
            }
            break;

        case logic_gate_flipflop: //  = 6,
            exit_hook(stderr,
                    "logic_gate_add_input : cannot add input to flipflop type. "
                    "use 'logic_gate_flipflop_set_data' and 'logic_gate_flipflop_set_clock' instead\n");

        case logic_gate_none:     //  = 7    for pre-allocated gate types
        case logic_gate_constant: //  = 8    constant output never changes
        case logic_gate_signal:   //  = 9    module input type
            exit_hook(stderr,
                    "logic_gate_add_input : cannot add input to '%s' type\n", 
                    logic_gate_type_as_string(g));

        case logic_gate_forward:  //  = 10, // used as an optimization technique
            if(g->_forward.input_gate == NULL) {
                g->_forward.input_gate = input;
            }
            else {
                exit_hook(stderr, 
                        "logic_gate_add_input : cannot set input on 'forward' type more than once\n");
            }
            break;

        case logic_gate_buffer:   //  = 11, // tri-state
            exit_hook(stderr,
                    "logic_gate_add_input : cannot set input on 'tristate buffer' type. "
                    "use logic_gate_tristate_set_data and logic_gate_tristate_set_enable\n");
            
        case logic_gate_wire:     //  = 12, // passive wire type 
            {
                logic_input_t* inp_ptr = logic_input_alloc();
                inp_ptr->output_ptr = input;
                inp_ptr->next       = g->wire.input_list;
                g->wire.input_list = inp_ptr;
            }
            break;

        default:
            exit_hook(stderr, 
                    "logic_gate_add_input : unknown internal error -> invalid gate type '%d'\n", g->gate_type);
            exit(1);
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
    logic_gate_t* gptr = logic_gate_alloc_forward();
    return logic_gate_init(gptr, logic_gate_forward);
}

logic_gate_t* WIRE(void) {
    logic_gate_t* gptr = logic_gate_alloc();
    return logic_gate_init(gptr, logic_gate_wire);
}

logic_gate_t* TRISTATE_BUFFER(void) {
    logic_gate_t* gptr = logic_gate_alloc();
    return logic_gate_init(gptr, logic_gate_buffer);
}

logic_gate_t** BUS(int width) {
    logic_gate_t** bus_ptrs = (logic_gate_t**)malloc(sizeof(logic_gate_t*) * width);

    int i;
    for(i = 0; i < width; i++)
        bus_ptrs[i] = WIRE();

    return bus_ptrs;
}

#ifdef __cplusplus
} // extern C
#endif
