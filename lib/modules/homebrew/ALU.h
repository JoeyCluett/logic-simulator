#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

#include <lib/modules/LogicUnit.h>
#include <lib/modules/adders/CarryLookaheadAdder.h>
#include <lib/modules/Register.h>
#include <lib/modules/Multiplexer.h>

#include <lib/modules/homebrew/OverflowDetector.h>
#include <lib/modules/homebrew/ZeroDetector.h>

struct ALU_t {

    LogicUnit_t           logic_unit;
    CarryLookaheadAdder_t adder;
    Multiplexer_t         mux;
    Gate_t                mux_select;

    OverflowDetector_t overflow_detector;
    ZeroDetector_t     zero_detector;

    Register_t register_a;           // x16
    Register_t register_accumulator; // x16
    Register_t register_flags;       // x4

    Gate_t internal_flag_carry;    // forward
    Gate_t internal_flag_sign;     // forward
    Gate_t internal_flag_overflow; // forward
    Gate_t internal_flag_zero;     // forward

    Gate_t fn_add, fn_sub;
    Gate_t fn_and, fn_nand;
    Gate_t fn_or,  fn_nor;
    Gate_t fn_xor, fn_xnor;

    Gate_t input_forward[16]; // forward

    ALU_t(void);

    //
    // return all 4 flags in the expected order
    //
    std::vector<Gate_t> get_output_flags(void);

};

