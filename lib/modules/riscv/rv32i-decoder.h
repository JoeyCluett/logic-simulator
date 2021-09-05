#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>
#include <lib/modules/riscv/rv32i-instruction-register.h>
#include <lib/modules/riscv/rv32i-imm-gen.h>
#include <lib/modules/BitwiseEqualDetect.h>

struct RISCV_Decoder_t {

    RISCV_Instruction_Register_t ir;

    std::vector<Gate_t> instruction_bit_value; // FORWARD

    struct {

        BitwiseEqualDetect_t bed_0110111; // LUI
        BitwiseEqualDetect_t bed_0010111; // AUIPC
        BitwiseEqualDetect_t bed_1101111; // JAL
        BitwiseEqualDetect_t bed_1100111; // JALR
        BitwiseEqualDetect_t bed_1100011; // BEQ, BNE, BLT, BGE, BLTU, BGEU
        BitwiseEqualDetect_t bed_0000011; // LB, LH, LW, LBU, LHU
        BitwiseEqualDetect_t bed_0100011; // SB, SH, SW
        BitwiseEqualDetect_t bed_0010011; // ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI
        BitwiseEqualDetect_t bed_0110011; // ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND
        BitwiseEqualDetect_t bed_0001111; // FENCE
        //BitwiseEqualDetect_t bed_1110011; // ECALL, EBREAK

    } opcode_cmp;

    BitwiseEqualDetect_t bed_ecall;  // 32-bit
    BitwiseEqualDetect_t bed_ebreak; // ...

    struct { // FORWARD
        Gate_t LUI   ; Gate_t AUIPC ; Gate_t JAL   ; Gate_t JALR  ; Gate_t BEQ   ; 
        Gate_t BNE   ; Gate_t BLT   ; Gate_t BGE   ; Gate_t BLTU  ; Gate_t BGEU  ; 
        Gate_t LB    ; Gate_t LH    ; Gate_t LW    ; Gate_t LBU   ; Gate_t LHU   ; 
        Gate_t SB    ; Gate_t SH    ; Gate_t SW    ; Gate_t ADDI  ; Gate_t SLTI  ; 
        Gate_t SLTIU ; Gate_t XORI  ; Gate_t ORI   ; Gate_t ANDI  ; Gate_t SLLI  ; 
        Gate_t SRLI  ; Gate_t SRAI  ; Gate_t ADD   ; Gate_t SUB   ; Gate_t SLL   ; 
        Gate_t SLT   ; Gate_t SLTU  ; Gate_t XOR   ; Gate_t SRL   ; Gate_t SRA   ; 
        Gate_t OR    ; Gate_t AND   ; Gate_t FENCE ; Gate_t ECALL ; Gate_t EBREAK;
    } inst;

    struct {
        BitwiseEqualDetect_t bed_000;
        BitwiseEqualDetect_t bed_001;
        BitwiseEqualDetect_t bed_010;
        BitwiseEqualDetect_t bed_011;
        BitwiseEqualDetect_t bed_100;
        BitwiseEqualDetect_t bed_101;
        BitwiseEqualDetect_t bed_110;
        BitwiseEqualDetect_t bed_111;
    } funct_3_cmp;

    struct {
        BitwiseEqualDetect_t bed_0000000;
        BitwiseEqualDetect_t bed_0100000;
    } funct_7_cmp;


    RISCV_Immediates_t immediates;


    RISCV_Decoder_t(void);

    void set_data_in(std::vector<Gate_t> input);

    void set_clk(Gate_t clk);

    static void Test(void);

    void Verify(void);

};

const char* const instruction_name_lut[40] = {
    "LUI", 
    "AUIPC", 
    "JAL", 
    "JALR", 
    "BEQ", 
    "BNE", 
    "BLT", 
    "BGE", 
    "BLTU", 
    "BGEU", 
    "LB", 
    "LH", 
    "LW", 
    "LBU", 
    "LHU", 
    "SB", 
    "SH", 
    "SW", 
    "ADDI", 
    "SLTI", 
    "SLTIU", 
    "XORI", 
    "ORI", 
    "ANDI", 
    "SLLI", 
    "SRLI", 
    "SRAI", 
    "ADD", 
    "SUB", 
    "SLL", 
    "SLT", 
    "SLTU", 
    "XOR", 
    "SRL", 
    "SRA", 
    "OR", 
    "AND", 
    "FENCE", 
    "ECALL", 
    "EBREAK",
};


