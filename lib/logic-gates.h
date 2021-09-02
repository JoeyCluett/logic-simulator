#ifndef LOGIC_GATES
#define LOGIC_GATES

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum logic_type {

    logic_gate_and       = 0,
    logic_gate_nand      = 1,
    logic_gate_or        = 2,
    logic_gate_nor       = 3,
    logic_gate_xor       = 4,
    logic_gate_xnor      = 5,
    logic_gate_flipflop  = 6,
    logic_gate_none      = 7,  // for pre-allocated gate types, never used directly
    logic_gate_constant  = 8,
    logic_gate_signal    = 9,  // input type
    logic_gate_forward   = 10, // used as an optimization technique
    logic_gate_buffer    = 11, // tri-state buffer
    logic_gate_wire      = 12, // passive wire type
    logic_gate_re_detect = 13, // rising-edge detector, callback used during update phase
} logic_type;

// can go in header because they are const
const int OUTPUT_ZERO = 0;
const int OUTPUT_ONE  = 1;
const int OUTPUT_HI_Z = 2;
const int OUTPUT_UND  = 3;

// for the rising edge detector
typedef void(*re_detect_callback)(void);

// forward declare input type
struct logic_input_t;

typedef struct logic_gate_t {

    int output_value;
    logic_type gate_type;

    union {

        struct {
            struct logic_input_t* inputs;
            int fetched_value;
            int n_inputs;
            int max_inputs;
        } gate;

        struct {
            
            // flip flops are allowed only one clock and one data input
            struct logic_gate_t* clk_input;
            struct logic_gate_t* data_input;

            int prev_clk_value;
            int current_clk_value;
            int data_input_value;

        } flipflop;

        // forward types need to be fully evaluated every simulation cycle. this means the forward
        // chain started by _forward.input_gate must be followed until another logic type is encountered
        struct {
            //struct logic_input_t* buffer_list;
            struct logic_gate_t* input_gate;
            int fetched_value;
        } _forward;

        struct {

            struct logic_gate_t* data_input;
            struct logic_gate_t* select_input;

            int fetched_data;
            int fetched_select;

        } tristate_buffer;

        struct {
            struct logic_input_t* input_list;
            int fetched_data;
        } wire;

        struct {
            struct logic_gate_t* input;
            int prev_value;
            int current_value;
            re_detect_callback _callback;
        } re_detect;

    };

} logic_gate_t;

// user will never explicitly initialize one of these
typedef struct logic_input_t {
    struct logic_input_t* next; // creates linked list
    logic_gate_t* output_ptr;
} logic_input_t;

// heap allocate gate and initialize
// used only for i/o signal types
logic_gate_t* logic_gate_new(logic_type type);

// initialize a pre-allocated gate with specific type
logic_gate_t* logic_gate_init(logic_gate_t* g, logic_type type);

// heap allocate logic_input_t struct
logic_input_t* logic_input_new(void);

// set the output of gate
void logic_gate_signal_set(logic_gate_t* g, int value);

// verify various aspects of all allocated gates
void logic_verify_environment(void);

// a few globals, initialized elsewhere
extern logic_gate_t* ONE;
extern logic_gate_t* ZERO;
extern logic_gate_t* HI_Z;
extern logic_gate_t* UND;

#ifdef __cplusplus
}
#endif

#endif // LOGIC_GATES
