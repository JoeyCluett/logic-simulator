#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

#include <lib/modules/riscv/rv32i-decoder.h>
#include <lib/modules/riscv/rv32i-register-file.h>

#include <vector>

struct RISCV_Cpu_t {

    RISCV_Decoder_t      decoder;
    RISCV_RegisterFile_t register_file;


    std::vector<Gate_t>  memory_bus; // WIRE x32

    RISCV_Cpu_t(void);

};
