#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

struct ZeroExtend_t {

    std::vector<Gate_t> input_forwards;
    std::vector<Gate_t> output_forwards;

    ZeroExtend_t(size_t input_width, size_t output_width);
    void set_input(std::vector<Gate_t> in);
    auto get_output(void) -> std::vector<Gate_t>;

};

