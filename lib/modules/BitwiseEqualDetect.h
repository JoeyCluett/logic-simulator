#pragma once

#include <lib/cpp-api/gate-wrapper.h>
#include <lib/logic.h>
#include <vector>

struct BitwiseEqualDetect_t {

    Gate_t andgate;
    std::vector<Gate_t> xorgates;
    std::vector<Gate_t> forward_a;
    std::vector<Gate_t> forward_b;

    BitwiseEqualDetect_t(size_t width);

    Gate_t get_output(void);

    void set_a_inputs(std::vector<Gate_t> a);

    void set_b_inputs(std::vector<Gate_t> b);

    operator Gate_t();

};
