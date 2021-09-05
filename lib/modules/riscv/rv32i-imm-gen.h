#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>
#include <lib/modules/BitwiseEqualDetect.h>

enum class RV32I_Imm : int {
    _11_0    = 0,
    _11_5    = 1,
    _12_10_5 = 2,
    _4_0     = 3,
    _4_1_11  = 4,
    _31_12   = 5,
    _20_10_1_11_19_12 = 6
};

struct RISCV_Immediates_t {

    std::vector<Gate_t> bus; // WIRE
    std::vector<Gate_t> input_fwd_vec; // FORWARD

    // vectors of tristate buffers
    std::vector<Gate_t> buffer_imm_11_0;             // imm[11:0]
    std::vector<Gate_t> buffer_imm_11_5;             // imm[11:5]
    std::vector<Gate_t> buffer_imm_12_10_5;          // imm[12, 10:5]
    std::vector<Gate_t> buffer_imm_4_0;              // imm[4:0]
    std::vector<Gate_t> buffer_imm_4_1_11;           // imm[4:1, 11]
    std::vector<Gate_t> buffer_imm_31_12;            // imm[31:12]
    std::vector<Gate_t> buffer_imm_20_10_1_11_19_12; // imm[20, 10:1, 11, 19:12]

    std::vector<Gate_t> se_imm_11_0;             // 20 bit
    std::vector<Gate_t> se_imm_11_5;             // 20 bit
    std::vector<Gate_t> se_imm_12_10_5;          // 19 bit
    std::vector<Gate_t> se_imm_20_10_1_11_19_12; // 11 bit

    std::vector<Gate_t> zero_pad_imm_31_12; // 12 bit
    Gate_t zero_pad_imm_20_10_1_11_19_12;   // 1 bit
    Gate_t zero_pad_imm_4_1_11;             // 1 bit

    // forwards
    Gate_t fwd_sel_imm_11_0;
    Gate_t fwd_sel_imm_11_5;
    Gate_t fwd_sel_imm_12_10_5;
    Gate_t fwd_sel_imm_4_0;
    Gate_t fwd_sel_imm_4_1_11;
    Gate_t fwd_sel_imm_31_12;
    Gate_t fwd_sel_imm_20_10_1_11_19_12;

    RISCV_Immediates_t(void);

    void set_select(RV32I_Imm imm, Gate_t g);

    auto get_output_bus(void) -> std::vector<Gate_t>;

    void set_input_bus(std::vector<Gate_t> input_bus);

    void Verify(void);

};
