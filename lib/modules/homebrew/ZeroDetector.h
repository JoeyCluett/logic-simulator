#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

#include <vector>

struct ZeroDetector_t {

    Gate_t norgate; // nor
    Gate_t input_forwards[16];

    ZeroDetector_t(void);

    //
    // get output gate
    //
    Gate_t get_output(void);

    //
    // set all 16 inputs for ZeroDetector
    //
    void set_input(std::vector<Gate_t> gates);

};

