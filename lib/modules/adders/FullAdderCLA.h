#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

struct FullAdder_CLA_t {

    Gate_t and_lower;
    Gate_t xor_lower;
    Gate_t xor_upper;

    Gate_t forward_A;
    Gate_t forward_B;
    Gate_t forward_Cin;

    FullAdder_CLA_t(void);

    void set_a(Gate_t a);
    void set_b(Gate_t b);
    void set_ci(Gate_t ci);
    Gate_t get_sum(void);

    Gate_t get_generate(void);
    Gate_t get_propagate(void);

    void VerifyInputs(void);

};
