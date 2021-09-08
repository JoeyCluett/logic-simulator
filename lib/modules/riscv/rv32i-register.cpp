#include <lib/modules/riscv/rv32i-register.h>

#include <lib/cpp-api/gate-wrapper.h>
#include <lib/cpp-api/range.h>

RISCV_Register_t::RISCV_Register_t(void) 
    :
        ffs(VectorInit_(32, FLIPFLOP)),
        clk_and(AND()),
        clk_fwd(FORWARD()),
        clk_en_fwd(FORWARD()),
        rs1_buf(VectorInit_(32, TRISTATE_BUFFER)),
        rs2_buf(VectorInit_(32, TRISTATE_BUFFER)),
        rs1_decode_fwd(FORWARD()),
        rs2_decode_fwd(FORWARD())
{

    this->clk_and.add_input(this->clk_fwd);
    this->clk_and.add_input(this->clk_en_fwd);

    for(auto& ff : this->ffs)
        ff.ff_set_clock(this->clk_and);

    VectorConnect_(this->rs1_buf, this->ffs);
    VectorConnect_(this->rs2_buf, this->ffs);    

    for(size_t i : range(0ul, 32ul)) {
        this->rs1_buf.at(i).tristate_set_enable(this->rs1_decode_fwd);
        this->rs2_buf.at(i).tristate_set_enable(this->rs2_decode_fwd);
    }

    

}

void RISCV_Register_t::set_data_in(std::vector<Gate_t> gv) {
    VectorConnect_(this->ffs, gv);
}

auto RISCV_Register_t::get_rs1_data_out(void) -> std::vector<Gate_t> {
    return this->rs1_buf;
}

auto RISCV_Register_t::get_rs2_data_out(void) -> std::vector<Gate_t> {
    return this->rs2_buf;
}

void RISCV_Register_t::set_clk(Gate_t g) {
    this->clk_fwd.add_input(g);
}

void RISCV_Register_t::set_clk_en(Gate_t g) {
    this->clk_en_fwd.add_input(g);
}

void RISCV_Register_t::set_rs1_decode(Gate_t g) {
    for(auto& oth : this->rs1_buf)
        oth.tristate_set_enable(g);
}

void RISCV_Register_t::set_rs2_decode(Gate_t g) {
    for(auto& oth : this->rs2_buf)
        oth.tristate_set_enable(g);
}

