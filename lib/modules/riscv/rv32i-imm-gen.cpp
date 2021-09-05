#include <lib/modules/riscv/rv32i-imm-gen.h>

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

#include <stdexcept>

RISCV_Immediates_t::RISCV_Immediates_t(void) 
    :

        bus(VectorInit_(32, WIRE)), // output bus

        input_fwd_vec(VectorInit_(32, FORWARD)),

        // buffers
        buffer_imm_11_0(             VectorInit_(12, TRISTATE_BUFFER)), // imm[11:0]
        buffer_imm_11_5(             VectorInit_( 7, TRISTATE_BUFFER)), // imm[11:5]
        buffer_imm_12_10_5(          VectorInit_( 7, TRISTATE_BUFFER)), // imm[12, 10:5]
        buffer_imm_4_0(              VectorInit_( 5, TRISTATE_BUFFER)), // imm[4:0]
        buffer_imm_4_1_11(           VectorInit_( 5, TRISTATE_BUFFER)), // imm[4:1, 11]
        buffer_imm_31_12(            VectorInit_(20, TRISTATE_BUFFER)), // imm[31:12]
        buffer_imm_20_10_1_11_19_12( VectorInit_(20, TRISTATE_BUFFER)), // imm[20, 10:1, 11, 19:12]

        // sign-extension buffers
        se_imm_11_0(             VectorInit_(20, TRISTATE_BUFFER)), // 20 bit
        se_imm_11_5(             VectorInit_(20, TRISTATE_BUFFER)), // 20 bit
        se_imm_12_10_5(          VectorInit_(19, TRISTATE_BUFFER)), // 19 bit
        se_imm_20_10_1_11_19_12( VectorInit_(11, TRISTATE_BUFFER)), // 11 bit

        // zero-padding buffers
        zero_pad_imm_31_12(VectorInit_(12, TRISTATE_BUFFER)), // 12 bit
        zero_pad_imm_20_10_1_11_19_12(TRISTATE_BUFFER()),     // 1 bit
        zero_pad_imm_4_1_11(TRISTATE_BUFFER()),               // 1 bit

        // select forwards
        fwd_sel_imm_11_0(FORWARD()),
        fwd_sel_imm_11_5(FORWARD()),
        fwd_sel_imm_12_10_5(FORWARD()),
        fwd_sel_imm_4_0(FORWARD()),
        fwd_sel_imm_4_1_11(FORWARD()),
        fwd_sel_imm_31_12(FORWARD()),
        fwd_sel_imm_20_10_1_11_19_12(FORWARD())
{

    auto set_enable_for_vec = [](std::vector<Gate_t>& gv, Gate_t en) -> void {
        for(auto& g : gv) {
            g.tristate_set_enable(en);
        }
    };

    auto set_data_for_vec = [](std::vector<Gate_t>& gv, Gate_t d) -> void {
        for(auto& g : gv) {
            g.tristate_set_data(d);
        }
    };

    // connect enable lines for various immediate chunks
    set_enable_for_vec(this->buffer_imm_11_0,             this->fwd_sel_imm_11_0);
    set_enable_for_vec(this->buffer_imm_11_5,             this->fwd_sel_imm_11_5);
    set_enable_for_vec(this->buffer_imm_12_10_5,          this->fwd_sel_imm_12_10_5);
    set_enable_for_vec(this->buffer_imm_4_0,              this->fwd_sel_imm_4_0);
    set_enable_for_vec(this->buffer_imm_4_1_11,           this->fwd_sel_imm_4_1_11);
    set_enable_for_vec(this->buffer_imm_31_12,            this->fwd_sel_imm_31_12);
    set_enable_for_vec(this->buffer_imm_20_10_1_11_19_12, this->fwd_sel_imm_20_10_1_11_19_12);

    set_enable_for_vec(this->se_imm_11_0,             this->fwd_sel_imm_11_0);
    set_enable_for_vec(this->se_imm_11_5,             this->fwd_sel_imm_11_5);
    set_enable_for_vec(this->se_imm_12_10_5,          this->fwd_sel_imm_12_10_5);
    set_enable_for_vec(this->se_imm_20_10_1_11_19_12, this->fwd_sel_imm_20_10_1_11_19_12);

    set_enable_for_vec(this->zero_pad_imm_31_12, this->fwd_sel_imm_31_12);
    this->zero_pad_imm_20_10_1_11_19_12.tristate_set_enable(this->fwd_sel_imm_20_10_1_11_19_12);
    this->zero_pad_imm_4_1_11.tristate_set_enable(this->fwd_sel_imm_4_1_11);

    // connect data lines for various immediate chunks
    set_data_for_vec(this->se_imm_11_0,             this->input_fwd_vec.back());
    set_data_for_vec(this->se_imm_11_5,             this->input_fwd_vec.back());
    set_data_for_vec(this->se_imm_12_10_5,          this->input_fwd_vec.back());
    set_data_for_vec(this->se_imm_20_10_1_11_19_12, this->input_fwd_vec.back());

    set_data_for_vec(this->zero_pad_imm_31_12, ZERO);
    this->zero_pad_imm_20_10_1_11_19_12.tristate_set_data(ZERO);
    this->zero_pad_imm_4_1_11.tristate_set_data(ZERO);

    // connect sign extension and zero padding chunk buffers to output bus
    VectorConnect_(VectorSlice_(this->bus, 12, 31+1), this->se_imm_11_0);
    VectorConnect_(VectorSlice_(this->bus, 12, 31+1), this->se_imm_11_5);
    VectorConnect_(VectorSlice_(this->bus, 13, 31+1), this->se_imm_12_10_5);
    VectorConnect_(VectorSlice_(this->bus, 21, 31+1), this->se_imm_20_10_1_11_19_12);

    this->bus.at(0).add_input(this->zero_pad_imm_20_10_1_11_19_12);
    this->bus.at(0).add_input(this->zero_pad_imm_4_1_11);
    VectorConnect_(VectorSlice_(this->bus, 0, 11+1), this->zero_pad_imm_31_12);

    // connect input bus to buffers
    VectorConnect_(this->buffer_imm_11_0,    VectorSlice_(this->input_fwd_vec, 20, 31+1));
    VectorConnect_(this->buffer_imm_11_5,    VectorSlice_(this->input_fwd_vec, 25, 31+1));
    VectorConnect_(this->buffer_imm_12_10_5, VectorSlice_(this->input_fwd_vec, 25, 31+1));
    VectorConnect_(this->buffer_imm_4_0,     VectorSlice_(this->input_fwd_vec,  7, 11+1));
    VectorConnect_(this->buffer_imm_4_1_11,  VectorSlice_(this->input_fwd_vec,  7, 11+1));
    VectorConnect_(this->buffer_imm_31_12,   VectorSlice_(this->input_fwd_vec, 12, 31+1));
    VectorConnect_(this->buffer_imm_20_10_1_11_19_12, VectorSlice_(this->input_fwd_vec, 12, 31+1));

    { // connect buffers to output bus

        std::vector<Gate_t> output_slice;

        VectorConnect_(VectorSlice_(this->bus, 0, 11+1), this->buffer_imm_11_0);

        VectorConnect_(VectorSlice_(this->bus, 5, 11+1), this->buffer_imm_11_5);

        this->bus.at(12).add_input(this->buffer_imm_12_10_5.back());
        VectorConnect_(VectorSlice_(this->bus, 5, 10+1), VectorSlice_(this->buffer_imm_12_10_5, 0, 6));

        VectorConnect_(VectorSlice_(this->bus, 0, 4+1), this->buffer_imm_4_0);

        this->bus.at(11).add_input(this->buffer_imm_4_1_11.at(0));
        VectorConnect_(VectorSlice_(this->bus, 1, 4+1), VectorSlice_(this->buffer_imm_4_1_11, 1, 4+1));

        VectorConnect_(VectorSlice_(this->bus, 12, 31+1), this->buffer_imm_31_12);

        this->bus.at(20).add_input(this->buffer_imm_20_10_1_11_19_12.at(19));
        this->bus.at(11).add_input(this->buffer_imm_20_10_1_11_19_12.at(8));
        VectorConnect_(VectorSlice_(this->bus, 1, 10+1), VectorSlice_(this->buffer_imm_20_10_1_11_19_12, 9, 18+1));
        VectorConnect_(VectorSlice_(this->bus, 12, 19+1), VectorSlice_(this->buffer_imm_20_10_1_11_19_12, 0, 7+1));
    }
}

void RISCV_Immediates_t::set_select(RV32I_Imm imm, Gate_t g) {
    switch(imm) {
        case RV32I_Imm::_11_0:             this->fwd_sel_imm_11_0.add_input(g);             break;
        case RV32I_Imm::_11_5:             this->fwd_sel_imm_11_5.add_input(g);             break;
        case RV32I_Imm::_12_10_5:          this->fwd_sel_imm_12_10_5.add_input(g);          break;
        case RV32I_Imm::_4_0:              this->fwd_sel_imm_4_0.add_input(g);              break;
        case RV32I_Imm::_4_1_11:           this->fwd_sel_imm_4_1_11.add_input(g);           break;
        case RV32I_Imm::_31_12:            this->fwd_sel_imm_31_12.add_input(g);            break;
        case RV32I_Imm::_20_10_1_11_19_12: this->fwd_sel_imm_20_10_1_11_19_12.add_input(g); break;
        default:
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " : invalid immediate specifier");
    }
}

auto RISCV_Immediates_t::get_output_bus(void) -> std::vector<Gate_t> {
    return this->bus;
}

void RISCV_Immediates_t::set_input_bus(std::vector<Gate_t> input_bus) {
    VectorConnect_(this->input_fwd_vec, input_bus);
}

void RISCV_Immediates_t::Verify(void) {

    GATE_VEC_TEST(bus);
    GATE_VEC_TEST(input_fwd_vec);

    GATE_VEC_TEST(buffer_imm_11_0);
    GATE_VEC_TEST(buffer_imm_11_5);
    GATE_VEC_TEST(buffer_imm_12_10_5);
    GATE_VEC_TEST(buffer_imm_4_0);
    GATE_VEC_TEST(buffer_imm_4_1_11);
    GATE_VEC_TEST(buffer_imm_31_12);
    GATE_VEC_TEST(buffer_imm_20_10_1_11_19_12);

    GATE_VEC_TEST(se_imm_11_0);
    GATE_VEC_TEST(se_imm_11_5);
    GATE_VEC_TEST(se_imm_12_10_5);
    GATE_VEC_TEST(se_imm_20_10_1_11_19_12);

    GATE_VEC_TEST(zero_pad_imm_31_12);
    GATE_TEST(zero_pad_imm_20_10_1_11_19_12);
    GATE_TEST(zero_pad_imm_4_1_11);

    GATE_TEST(fwd_sel_imm_11_0);
    GATE_TEST(fwd_sel_imm_11_5);
    GATE_TEST(fwd_sel_imm_12_10_5);
    GATE_TEST(fwd_sel_imm_4_0);
    GATE_TEST(fwd_sel_imm_4_1_11);
    GATE_TEST(fwd_sel_imm_31_12);
    GATE_TEST(fwd_sel_imm_20_10_1_11_19_12);
}
