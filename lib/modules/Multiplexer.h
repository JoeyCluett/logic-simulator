#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

#include <vector>

struct Multiplexer_t {

    std::vector<Gate_t> one_gates;
    std::vector<Gate_t> zero_gates;
    std::vector<Gate_t> or_gates;
    Gate_t not_gate;     // NOT ... obviously
    Gate_t select_input; // FORWARD

    //
    // default ctor connects all internal components
    //
    Multiplexer_t(size_t width);

    //
    // returns vector of output OR gates
    //
    std::vector<Gate_t> get_output(void);

    //
    // set inputs active when enable is zero
    //
    void set_zero_input(std::vector<Gate_t> inp);

    //
    // set single input active when enable is zero
    //
    void set_zero_input(size_t idx, Gate_t inp);

    //
    // set inputs active when enable is one
    //
    void set_one_input(std::vector<Gate_t> inp);

    //
    // set single input active when enable is one
    //
    void set_one_input(size_t idx, Gate_t inp);

    //
    // set mux select input
    //
    void set_select_input(Gate_t gate);

};


