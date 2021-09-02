#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>
#include <lib/modules/adders/HalfAdder.h>

// full adder composed of two half adders
struct FullAdder_t {
    HalfAdder_t upper;
    HalfAdder_t lower;
    Gate_t carry_out;

    Gate_t cin_forward;

    FullAdder_t(void);

    void set_a(Gate_t a);
    void set_b(Gate_t b);
    void set_ci(Gate_t ci);
    Gate_t get_co(void);
    Gate_t get_sum(void);

    // for CLA-type adders
    Gate_t get_cla_generate(void);
    Gate_t get_cla_propagate(void);

};
