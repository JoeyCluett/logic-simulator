#ifndef LOGIC_GATES
#define LOGIC_GATES

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum logic_type {

    logic_gate_and  = 0,
    logic_gate_nand = 1,
    logic_gate_or   = 2,
    logic_gate_nor  = 3,
    logic_gate_xor  = 4,
    logic_gate_xnor = 5,
    logic_gate_flipflop  = 6,
    logic_gate_none      = 7,  // for pre-allocated gate types
    logic_gate_constant  = 8,
    logic_gate_signal    = 9,  // input type
    logic_gate_forward   = 10, // used as an optimization technique
    logic_gate_buffer = 11,
    logic_gate_bus    = 12,

} logic_type;

// forward declare input type
struct logic_input_t;

typedef struct logic_gate_t {

    int output_value;
    logic_type gate_type;

    union {

        struct {
            
            // flip flops are allowed only one clock and one data input
            struct logic_gate_t* clk_input;
            struct logic_gate_t* data_input;

            int prev_clk_value;
            int current_clk_value;
            int data_input_value;

        } flipflop;

        struct {

            struct logic_input_t* inputs;

            int fetched_value;
            int n_inputs;
            int n_high;
            int n_low;

        } gate;

        // _forward gate is a lil odd. the 'input list' is actually all of the gates directly downstream from this gate.
        // _forward will buffer them until its own input is evaluated at which point the input gate is 'forwarded' to 
        // the buffered gate references and those references are released
        struct {

            struct logic_input_t* buffer_list;
            struct logic_gate_t* input_gate;
            struct logic_gate_t* next_forward_gate; // link multiple _forward gates if needed

        } _forward;

    };

} logic_gate_t;

// user will never explicitly initialize one of these
typedef struct logic_input_t {
    struct logic_input_t* next; // creates linked list
    logic_gate_t* output_ptr;
} logic_input_t;

// heap allocate gate and initialize
logic_gate_t* logic_gate_new(logic_type type);

// initialize a pre-allocated gate with specific type
logic_gate_t* logic_gate_init(logic_gate_t* g, logic_type type);

// heap allocate logic_input_t struct
logic_input_t* logic_input_new(void);

// set the output of gate
void logic_gate_signal_set(logic_gate_t* g, int value);

// a few globals, initialized elsewhere
extern logic_gate_t* ONE;
extern logic_gate_t* ZERO;


#ifdef __cplusplus
}
#endif

#endif // LOGIC_GATES
