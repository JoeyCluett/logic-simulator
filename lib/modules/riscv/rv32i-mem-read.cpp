#include <lib/modules/riscv/rv32i-mem-read.h>

#include <lib/cpp-api/range.h>

RISCV_MemoryReadUnit_t::RISCV_MemoryReadUnit_t(void) 
    :
        input_bus_fwd(VectorInit_(32, FORWARD)),

        hword_sel_mux(1, 2, 16),
        byte_sel_mux(2, 4, 8),

        hword_out_se(16, 32),
        hword_out_ze(16, 32),
        byte_out_se(8, 32),
        byte_out_ze(8, 32),

        hword_buf(VectorInit_(32, TRISTATE_BUFFER)),
        hwordu_buf(VectorInit_(32, TRISTATE_BUFFER)),
        byte_buf(VectorInit_(32, TRISTATE_BUFFER)),
        byteu_buf(VectorInit_(32, TRISTATE_BUFFER)),
        word_buf(VectorInit_(32, TRISTATE_BUFFER)),

        hword_buf_out(FORWARD()),
        hwordu_buf_out(FORWARD()),
        byte_buf_out(FORWARD()),
        byteu_buf_out(FORWARD()),
        word_buf_out(FORWARD())
{

    for(size_t i : range(0ul, 32ul)) {
        this->hword_buf.at(i).tristate_set_enable(this->hword_buf_out);
        this->hwordu_buf.at(i).tristate_set_enable(this->hwordu_buf_out);
        this->byte_buf.at(i).tristate_set_enable(this->byte_buf_out);
        this->byteu_buf.at(i).tristate_set_enable(this->byteu_buf_out);
        this->word_buf.at(i).tristate_set_enable(this->word_buf_out);
    }

    VectorConnect_(this->hword_buf,  this->hword_out_se.get_output());
    VectorConnect_(this->hwordu_buf, this->hword_out_ze.get_output());
    VectorConnect_(this->byte_buf,   this->byte_out_se.get_output());
    VectorConnect_(this->byteu_buf,  this->byte_out_ze.get_output());

    this->hword_out_se.set_input(this->hword_sel_mux.get_output());
    this->hword_out_ze.set_input(this->hword_sel_mux.get_output());
    this->byte_out_se.set_input(this->byte_sel_mux.get_output());
    this->byte_out_ze.set_input(this->byte_sel_mux.get_output());

    this->hword_sel_mux.set_input(0, VectorSlice_(this->input_bus_fwd,  0, 16));
    this->hword_sel_mux.set_input(1, VectorSlice_(this->input_bus_fwd, 16, 32));

    this->byte_sel_mux.set_input(0, VectorSlice_(this->input_bus_fwd,  0,  8));
    this->byte_sel_mux.set_input(1, VectorSlice_(this->input_bus_fwd,  8, 16));
    this->byte_sel_mux.set_input(2, VectorSlice_(this->input_bus_fwd, 16, 24));
    this->byte_sel_mux.set_input(3, VectorSlice_(this->input_bus_fwd, 24, 32));
}

void RISCV_MemoryReadUnit_t::set_input_bus(std::vector<Gate_t> in_bus) {
    VectorConnect_(this->input_bus_fwd, in_bus);
}


