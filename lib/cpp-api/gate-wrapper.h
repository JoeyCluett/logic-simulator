#pragma once

#include <lib/logic.h>
#include <vector>
#include <string>

struct Gate_t {
    logic_gate_t* internal_gate;

    //
    // default ctor
    //
    Gate_t(void);

    //
    // specify a logic_gate_t* type at initialization
    //
    Gate_t(logic_gate_t* gate_ptr);

    //
    // set the output value for this Gate_t
    // calls into C API
    //
    void set_output(int val);

    //
    // get the current output value of this Gate_t
    //
    int get_output(void);

    //
    // sets a logic_gate_t* to be used by this wrapper
    //
    void set(logic_gate_t* gate_ptr);

    //
    // add logic gate input to this gate
    // calls into existing C API
    //
    void add_input(Gate_t gate);

    //
    // returns the number of logic gate inputs
    //
    int num_inputs(void);

    //
    // set clock input for internal FlipFlop type
    // error checking done by C API
    //
    void ff_set_clock(Gate_t gate);

    //
    // set data input for internal FlipFlop type
    // error checking done by C API
    //
    void ff_set_data(Gate_t gate);

    //
    // set data input for internal tristate buffer type
    // error checking done by C API
    //
    void tristate_set_data(Gate_t gate);

    //
    // set enable input for internal tristate buffer type
    // error checking done by C API
    //
    void tristate_set_enable(Gate_t gate);

    //
    // check if Gate_t has any inputs
    //
    bool has_inputs(void);

    //
    // cast Gate_t to underlying pointer type
    //
    typedef logic_gate_t* logic_gate_ptr_t;
    operator logic_gate_ptr_t();

};

Gate_t operator&(Gate_t lhs, Gate_t rhs);
Gate_t operator|(Gate_t lhs, Gate_t rhs);
Gate_t operator^(Gate_t lhs, Gate_t rhs);
Gate_t operator~(Gate_t g);

// wrap gate in a forward type
Gate_t ForwardWrap_(Gate_t g);

template<typename CallableInitFn>
std::vector<Gate_t> VectorInit_(size_t sz, CallableInitFn init_fn) {
    std::vector<Gate_t> v;
    for(size_t i = 0UL; i < sz; i++)
        v.push_back(init_fn());
    return v;
}

std::vector<Gate_t> VectorInit_(const std::string& s);
std::vector<Gate_t> VectorInit_(size_t width, size_t constant);
std::vector<Gate_t> VectorInit_(int width, size_t constant);
std::vector<Gate_t> VectorInit_(size_t width, int constant);
std::vector<Gate_t> VectorInit_(int width, int constant);

std::vector<logic_gate_t*> VectorCvt_(std::vector<Gate_t> v);

void VectorConnect_(
    std::vector<Gate_t> dst, 
    std::vector<Gate_t> src);

auto VectorSlice_(std::vector<Gate_t>& src, size_t start, size_t end) -> std::vector<Gate_t>;

template<typename VectorType>
void v_VectorCollate_(VectorType& base, Gate_t other) {
    base.push_back(other);
}

template<typename VectorType>
void v_VectorCollate_(VectorType& base, std::vector<Gate_t> other) {
    for(auto& g : other)
        base.push_back(g);
}

template<typename VectorType, typename NextType, typename... OtherTypes>
void v_VectorCollate_(VectorType& base, NextType next, OtherTypes... others) {
    v_VectorCollate_(base, next);
    v_VectorCollate_(base, others...);
}

template<typename... OtherTypes>
auto VectorCollate_(OtherTypes... others) -> std::vector<Gate_t> {
    std::vector<Gate_t> vec;
    v_VectorCollate_(vec, others...);
    return vec;
}


