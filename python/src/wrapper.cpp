#include <iostream>
#include <boost/python.hpp>

#include <logic-gates.h>
#include <logic-gate-utility.h>
#include <logic-gate-allocator.h>

#define RET_VPTR return_value_policy<return_opaque_pointer>()

// logic-gates.h interface functions
void* bp_logic_gate_new(int type);
void* bp_logic_gate_init(void* g, int type);
void* bp_logic_input_new(void);
void  bp_logic_gate_signal_set(void* g, int value);

// logic-gate-allocator.h interface functions
long int bp_logic_allocator_ngates(void);
void* bp_logic_gate_alloc(void);

// logic-gate-utility.h interface functions
void bp_logic_gate_add_input(void* g, void* input);
void bp_logic_gate_flipflop_set_clock(void* g, void* clk);
void bp_logic_gate_flipflop_set_data(void* g, void* data);
int  bp_logic_gate_simulate(void);
//int  bp_logic_gate_simulate_callback(void(*callb)(void));

// get constants for zero and one
void* bp_get_constant_zero(void);
void* bp_get_constant_one(void);

// python only has pointers, use this to get data from structure pointer refers to
int bp_get_output_value(void* g);
int bp_get_type(void* g);
int bp_get_num_inputs(void* g);

BOOST_PYTHON_MODULE(hdl)
{
    using namespace boost::python;

    std::cout << "BOOST_PYTHON_MODULE(hdl)" << std::endl;
    std::cout << "initializing backend logic allocator" << std::endl;
    logic_init(); // initialize logic allocator

    def("logic_gate_new",        bp_logic_gate_new,  RET_VPTR);
    def("logic_gate_init",       bp_logic_gate_init, RET_VPTR);
    def("logic_input_new",       bp_logic_input_new, RET_VPTR);
    def("logic_gate_signal_set", bp_logic_gate_signal_set);

    def("logic_allocator_ngates", bp_logic_allocator_ngates);
    def("logic_gate_alloc",       bp_logic_gate_alloc, RET_VPTR);

    def("logic_gate_add_input", bp_logic_gate_add_input);
    def("logic_gate_flipflop_set_clock", bp_logic_gate_flipflop_set_clock);
    def("logic_gate_flipflop_set_data",  bp_logic_gate_flipflop_set_data);
    def("logic_gate_simulate",  bp_logic_gate_simulate);

    def("logic_gate_get_constant_zero", bp_get_constant_zero, RET_VPTR);
    def("logic_gate_get_constant_one",  bp_get_constant_one,  RET_VPTR);

    def("logic_gate_get_output_value", bp_get_output_value);
    def("logic_gate_get_type",         bp_get_type);
    def("logic_gate_get_num_inputs",   bp_get_num_inputs);
}

int bp_get_num_inputs(void* g) {
    auto* gptr = static_cast<logic_gate_t*>(g);
    assert(gptr->gate_type >= 0 && gptr->gate_type <= 5);

    auto* input_ptr = gptr->gate.inputs;
    int n_inputs = 0;

    while(input_ptr) {
        n_inputs++;
        input_ptr = input_ptr->next;
    }

    return n_inputs;
}

int  bp_logic_gate_simulate(void) {
    return logic_gate_simulate();
}

int bp_get_type(void* g) {
    return static_cast<logic_gate_t*>(g)->gate_type;
}

int bp_get_output_value(void* g) {
    return static_cast<logic_gate_t*>(g)->output_value;
}

void* bp_get_constant_zero(void) {
    return (void*)ZERO;
}

void* bp_get_constant_one(void) {
    return (void*)ONE;
}

void bp_logic_gate_add_input(void* g, void* input) {
    logic_gate_add_input((logic_gate_t*)g, (logic_gate_t*)input);
}

void bp_logic_gate_flipflop_set_clock(void* g, void* clk) {
    logic_gate_flipflop_set_clock((logic_gate_t*)g, (logic_gate_t*)clk);
}

void bp_logic_gate_flipflop_set_data(void* g, void* data) {
    logic_gate_flipflop_set_data((logic_gate_t*)g, (logic_gate_t*)data);
}

long int bp_logic_allocator_ngates(void) {
    return logic_allocator_ngates();
}

void* bp_logic_gate_alloc(void) {
    return (void*)logic_gate_alloc();
}

void* bp_logic_gate_new(int type) { 
    return (void*)logic_gate_new((logic_type)type);
}

void* bp_logic_gate_init(void* g, int type) {
    return (void*)logic_gate_init((logic_gate_t*)g, (logic_type)type);
}

void* bp_logic_input_new(void) {
    return (void*)logic_input_new();
}

void  bp_logic_gate_signal_set(void* g, int value) {
    logic_gate_signal_set((logic_gate_t*)g, value);
}

