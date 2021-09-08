#include <lib/modules/riscv/rv32i-register-file.h>

RISCV_RegisterFile_t::RISCV_RegisterFile_t(void) 
    :
        rs1_data(VectorInit_(32, WIRE)),
        rs2_data(VectorInit_(32, WIRE)),
        data_in(VectorInit_(32, FORWARD)),
        regs(32),
        clk_fwd(FORWARD())
{

    for(size_t idx = 1ul; idx < 32; idx++) this->regs.at(idx).set_data_in(this->data_in);
    for(size_t idx = 0ul; idx < 32; idx++) this->regs.at(0).ffs.at(idx).ff_set_data(ZERO);

    for(auto& r : this->regs) {
        VectorConnect_(this->rs1_data, r.get_rs1_data_out());
        VectorConnect_(this->rs2_data, r.get_rs2_data_out());
        r.set_clk(this->clk_fwd);
    }

    

}

