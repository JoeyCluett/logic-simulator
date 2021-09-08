#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

#include <lib/modules/MuxGeneric.h>
#include <lib/modules/SignExtend.h>
#include <lib/modules/ZeroExtend.h>

#include <vector>

struct RISCV_MemoryReadUnit_t {

    std::vector<Gate_t> input_bus_fwd;

    MuxGeneric_t hword_sel_mux;
    MuxGeneric_t byte_sel_mux;

    SignExtend_t hword_out_se;
    ZeroExtend_t hword_out_ze;

    SignExtend_t byte_out_se;
    ZeroExtend_t byte_out_ze;

    std::vector<Gate_t> hword_buf;
    std::vector<Gate_t> hwordu_buf;
    std::vector<Gate_t> byte_buf;
    std::vector<Gate_t> byteu_buf;
    std::vector<Gate_t> word_buf;

    // FORWARDs for buf select inputs
    Gate_t hword_buf_out;
    Gate_t hwordu_buf_out;
    Gate_t byte_buf_out;
    Gate_t byteu_buf_out;
    Gate_t word_buf_out;

    RISCV_MemoryReadUnit_t(void);

    void set_input_bus(std::vector<Gate_t> in_bus);

};
