#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

struct OverflowDetector_t {

    Gate_t x;   // xor
    Gate_t xn;  // xnor
    Gate_t a0;  // and
    Gate_t a1;  // and
    Gate_t a2;  // and
    Gate_t out; // or

    Gate_t add;
    Gate_t sub;
    Gate_t sign_a;
    Gate_t sign_b;
    Gate_t sign_f;

    Gate_t sign_a_inv; // not
    Gate_t sign_b_inv; // not
    Gate_t sign_f_inv; // not

    //
    // default ctor connects internal elements
    //
    OverflowDetector_t(void);

    //
    // get output gate
    //
    Gate_t get_output(void);

    //
    // add inputs to the various forward gates used
    //
    void set_signal_add(Gate_t gate);    // signal that addition is being performed
    void set_signal_sub(Gate_t gate);    // signal that subtraction is being performed
    void set_signal_sign_a(Gate_t gate); 
    void set_signal_sign_b(Gate_t gate);
    void set_signal_sign_f(Gate_t gate);

    static void Test(void);

};
