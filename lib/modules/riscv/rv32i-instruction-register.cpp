#include <lib/modules/riscv/rv32i-instruction-register.h>

RISCV_InstructionRegister_t::RISCV_InstructionRegister_t(void) 
    :
        register_(VectorInit_(32, FLIPFLOP)),
        
        fields {
            VectorInit_(7, FORWARD), // opcode
            VectorInit_(5, FORWARD), // rd
            VectorInit_(5, FORWARD), // rs1
            VectorInit_(5, FORWARD), // rs2
            VectorInit_(3, FORWARD), // funct_3
            VectorInit_(7, FORWARD), // funct_7

            VectorInit_(12, FORWARD), //imm_11_0;
            VectorInit_(7,  FORWARD), //imm_11_5;
            VectorInit_(7,  FORWARD), //imm_12_10_5;
            VectorInit_(5,  FORWARD), //imm_4_0;
            VectorInit_(5,  FORWARD), //imm_4_1_11;
            VectorInit_(20, FORWARD), //imm_31_12;
            VectorInit_(20, FORWARD), //imm_20_10_1_11_19_12;
        },

        clk_input(FORWARD())
        
    {

    VectorConnect_(this->fields.opcode,  VectorSlice_(this->register_, 0,   6+1));
    VectorConnect_(this->fields.rd,      VectorSlice_(this->register_, 7,  11+1));
    VectorConnect_(this->fields.rs1,     VectorSlice_(this->register_, 15, 19+1));
    VectorConnect_(this->fields.rs2,     VectorSlice_(this->register_, 20, 24+1));
    VectorConnect_(this->fields.funct_3, VectorSlice_(this->register_, 12, 14+1));
    VectorConnect_(this->fields.funct_7, VectorSlice_(this->register_, 25, 31+1));

    VectorConnect_(this->fields.imm_11_0,    VectorSlice_(this->register_, 20, 31+1));
    VectorConnect_(this->fields.imm_11_5,    VectorSlice_(this->register_, 25, 31+1));
    VectorConnect_(this->fields.imm_12_10_5, VectorSlice_(this->register_, 25, 31+1));
    VectorConnect_(this->fields.imm_4_0,     VectorSlice_(this->register_,  7, 11+1));
    VectorConnect_(this->fields.imm_4_1_11,  VectorSlice_(this->register_,  7, 11+1));
    VectorConnect_(this->fields.imm_31_12,   VectorSlice_(this->register_, 12, 31+1));
    VectorConnect_(this->fields.imm_20_10_1_11_19_12, VectorSlice_(this->register_, 12, 31+1));

    for(size_t i = 0UL; i < 32; i++)
        this->register_[i].ff_set_clock(this->clk_input);

}

void RISCV_InstructionRegister_t::set_clk(Gate_t clk) {
    this->clk_input.add_input(clk);
}
