#pragma once

#include <vector>

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

struct WideAnd_t {

    Gate_t and_gate;

    //
    // default ctor
    //
    WideAnd_t(void);

    //
    // attach list of all gates to and_gate
    //
    void set(std::vector<Gate_t> gates);

    //
    // get the base output gate
    //
    Gate_t get_output(void);

};
