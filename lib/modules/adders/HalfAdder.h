#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

struct HalfAdder_t {

    Gate_t carry;
    Gate_t sum;
    Gate_t forward_a;
    Gate_t forward_b;

    HalfAdder_t(void);

    void set_a(Gate_t a);
    void set_b(Gate_t b);
    Gate_t get_carry(void);
    Gate_t get_sum(void);

};

