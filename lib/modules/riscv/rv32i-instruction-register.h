#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

#include <lib/modules/BitwiseEqualDetect.h>

struct RISCV_InstructionRegister_t {

    std::vector<Gate_t> register_;
    
    struct {

        // misc fields
        std::vector<Gate_t> opcode;
        std::vector<Gate_t> rd;
        std::vector<Gate_t> rs1;
        std::vector<Gate_t> rs2;
        std::vector<Gate_t> funct_3;
        std::vector<Gate_t> funct_7;

        // all immediate fields
        std::vector<Gate_t> imm_11_0;
        std::vector<Gate_t> imm_11_5;
        std::vector<Gate_t> imm_12_10_5;
        std::vector<Gate_t> imm_4_0;
        std::vector<Gate_t> imm_4_1_11;
        std::vector<Gate_t> imm_31_12;
        std::vector<Gate_t> imm_20_10_1_11_19_12;

    } fields;

    Gate_t clk_input;

    RISCV_InstructionRegister_t(void);

    void set_clk(Gate_t clk);

    void VerifyInputs(void);

};



