#ifndef LOGIC_GATE_UTILITY
#define LOGIC_GATE_UTILITY

#include "logic-gates.h"
#include "logic-gate-allocator.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef logic_gate_t*(*gate_alloc_fn)(void);

// return initialized gates. these work as wrappers over the 
// regular gate allocation/initialization functions. these 
// functions are the preferred way to allocate gates
logic_gate_t* AND(void);
logic_gate_t* NAND(void);
logic_gate_t* OR(void);
logic_gate_t* NOR(void);
logic_gate_t* XOR(void);
logic_gate_t* XNOR(void);
logic_gate_t* FLIPFLOP(void);
logic_gate_t* CONSTANT(int value);
logic_gate_t* SIGNAL(void);
logic_gate_t* FORWARD(void);
logic_gate_t* WIRE(void);
logic_gate_t* TRISTATE_BUFFER(void);
logic_gate_t** BUS(int width);

// adds gate input. input gate is pointer to another logic gate
void logic_gate_add_input(logic_gate_t* g, logic_gate_t* input);

// wrappers over logic_gate_t::flipflop functionality
void logic_gate_flipflop_set_clock(logic_gate_t* g, logic_gate_t* clk);
void logic_gate_flipflop_set_data(logic_gate_t* g, logic_gate_t* data);

// wrappers over logic_gate_t::tristate_buffer functionality
void logic_gate_tristate_set_data(logic_gate_t* g, logic_gate_t* data);
void logic_gate_tristate_set_enable(logic_gate_t* g, logic_gate_t* en);

// actual meat of the simulation occurs with this function. this function already 
// knows which gates are relevant to the simulation
int logic_gate_simulate(void);

// modified simulate function. calls the callback every cycle
int logic_gate_simulate_callback(void(*callb)(void));

// follow FORWARD types
logic_gate_t* logic_gate_follow(logic_gate_t* g);

// evaluate forward gate types and replace original FORWARD reference with actual gate reference
void logic_gate_eval_forwards(void);

#ifdef __cplusplus
} // extern C
#endif

#endif // LOGIC_GATE_UTILITY
