#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

#include <vector>

#include <lib/modules/DecoderWEnable.h>
#include <lib/modules/riscv/rv32i-register.h>

struct RISCV_RegisterFile_t {

    std::vector<Gate_t> rs1_data; // WIRE x32
    std::vector<Gate_t> rs2_data; // WIRE x32
    std::vector<Gate_t> data_in;  // FORWARD x32

    std::vector<RISCV_Register_t> regs; // x32 (special care with r0)

    Gate_t clk_fwd;

    RISCV_RegisterFile_t(void);

};
