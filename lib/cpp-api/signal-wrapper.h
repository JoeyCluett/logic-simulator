#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

#include <vector>

struct Signal_t {

    logic_gate_t* internal_gate;

    //
    // default ctor sets output to ZERO
    //
    Signal_t(void);

    //
    // ctor specifies a starting value for Signal_t
    //
    Signal_t(int initial_value);

    //
    // free underlying gate structure
    //
    ~Signal_t();

    //
    // set output value
    //
    void set(int value);

    //
    // cast signal to Gate_t type
    //
    operator Gate_t();

    //
    // cast to underlying logic_gate_t pointer
    //
    typedef logic_gate_t* logic_gate_ptr_t;
    operator logic_gate_ptr_t();

};

struct SignalVector_t {

    std::vector<Signal_t> signals;

    //
    // ctor specifies size of 
    //
    SignalVector_t(size_t width);

    //
    // typecast to std::vector of raw logic_gate_t pointers
    //
    operator std::vector<logic_gate_t*>();

    //
    // typecast to std::vector of raw logic_gate_t pointers
    //
    operator std::vector<Gate_t>();

    //
    // (unsigned long) assign the outputs in this vector with given bitwise information
    //
    void set_value(size_t i);

    //
    // (string) assign the outputs in this vector with given bitwise information
    //
    void set_value(const std::string& i);

    //
    // (string) assign the outputs in this vector with given bitwise information
    // first reverses the given string
    //
    void set_value_r(std::string i);

};
