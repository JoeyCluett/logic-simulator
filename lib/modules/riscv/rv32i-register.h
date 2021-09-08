#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

#include <vector>

struct RISCV_Register_t {

    std::vector<Gate_t> ffs; // FLIPFLOP x32

    Gate_t clk_and; // AND (clk, clk_en)
    Gate_t clk_fwd;
    Gate_t clk_en_fwd; // also rd_decode

    std::vector<Gate_t> rs1_buf;
    std::vector<Gate_t> rs2_buf;

    Gate_t rs1_decode_fwd;
    Gate_t rs2_decode_fwd;

    RISCV_Register_t(void);

    void set_data_in(std::vector<Gate_t> gv);

    auto get_rs1_data_out(void) -> std::vector<Gate_t>;
    auto get_rs2_data_out(void) -> std::vector<Gate_t>;

    void set_clk(Gate_t g);
    void set_clk_en(Gate_t g);

    void set_rs1_decode(Gate_t g);
    void set_rs2_decode(Gate_t g);

};
