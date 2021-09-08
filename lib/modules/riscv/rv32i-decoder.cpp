#include <lib/modules/riscv/rv32i-decoder.h>

#include <assert.h>
#include <sys/time.h>

#include <iostream>
#include <vector>
#include <tuple>

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>
#include <lib/cpp-api/gate-varargs.h>
#include <lib/cpp-api/signal-wrapper.h>
#include <lib/cpp-api/range.h>
#include <lib/modules/BitwiseEqualDetect.h>

RISCV_Decoder_t::RISCV_Decoder_t(void) 
    :
        instruction_bit_value(VectorInit_(40, FORWARD)),

        opcode_cmp {
            7, 7, 7, 7, 7, 7, 7, 7, 7, 7
        },

        bed_ecall(32),
        bed_ebreak(32),
        
        inst {
            FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD(),
            FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD(),
            FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD(),
            FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD(),
            FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD(), FORWARD()
        },

        funct_3_cmp {
            3, 3, 3, 3, 3, 3, 3, 3
        },

        funct_7_cmp {
            7, 7
        },

        decode_data {
            VectorInit_(5,  FORWARD), // RS1
            VectorInit_(5,  FORWARD), // RS2
            VectorInit_(5,  FORWARD), // RD
            VectorInit_(5,  FORWARD), // shamt
            VectorInit_(32, FORWARD), // imm32
            VectorInit_(40, FORWARD)  // inst
        }

    {

    opcode_cmp.bed_0110111.set_b_inputs(VectorInit_(7, 0b0110111)); // LUI
    opcode_cmp.bed_0010111.set_b_inputs(VectorInit_(7, 0b0010111)); // AUIPC
    opcode_cmp.bed_1101111.set_b_inputs(VectorInit_(7, 0b1101111)); // JAL
    opcode_cmp.bed_1100111.set_b_inputs(VectorInit_(7, 0b1100111)); // JALR
    opcode_cmp.bed_1100011.set_b_inputs(VectorInit_(7, 0b1100011)); // BEQ, BNE, BLT, BGE, BLTU, BGEU
    opcode_cmp.bed_0000011.set_b_inputs(VectorInit_(7, 0b0000011)); // LB, LH, LW, LBU, LHU
    opcode_cmp.bed_0100011.set_b_inputs(VectorInit_(7, 0b0100011)); // SB, SH, SW
    opcode_cmp.bed_0010011.set_b_inputs(VectorInit_(7, 0b0010011)); // ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI
    opcode_cmp.bed_0110011.set_b_inputs(VectorInit_(7, 0b0110011)); // ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND
    opcode_cmp.bed_0001111.set_b_inputs(VectorInit_(7, 0b0001111)); // FENCE

    funct_3_cmp.bed_000.set_b_inputs(VectorInit_(3, 0b000));
    funct_3_cmp.bed_001.set_b_inputs(VectorInit_(3, 0b001));
    funct_3_cmp.bed_010.set_b_inputs(VectorInit_(3, 0b010));
    funct_3_cmp.bed_011.set_b_inputs(VectorInit_(3, 0b011));
    funct_3_cmp.bed_100.set_b_inputs(VectorInit_(3, 0b100));
    funct_3_cmp.bed_101.set_b_inputs(VectorInit_(3, 0b101));
    funct_3_cmp.bed_110.set_b_inputs(VectorInit_(3, 0b110));
    funct_3_cmp.bed_111.set_b_inputs(VectorInit_(3, 0b111));

    funct_7_cmp.bed_0100000.set_b_inputs(VectorInit_(7, 0b0100000));
    funct_7_cmp.bed_0000000.set_b_inputs(VectorInit_(7, 0b0000000));

    bed_ecall.set_b_inputs(  VectorInit_(32, 0b00000000000000000000000001110011)); // ECALL
    bed_ebreak.set_b_inputs( VectorInit_(32, 0b00000000000100000000000001110011)); // EBREAK

    // all constants are connected to compare modules for opcodes
    // set the data inputs

    opcode_cmp.bed_0110111.set_a_inputs(this->ir.fields.opcode);
    opcode_cmp.bed_0010111.set_a_inputs(this->ir.fields.opcode);
    opcode_cmp.bed_1101111.set_a_inputs(this->ir.fields.opcode);
    opcode_cmp.bed_1100111.set_a_inputs(this->ir.fields.opcode);
    opcode_cmp.bed_1100011.set_a_inputs(this->ir.fields.opcode);
    opcode_cmp.bed_0000011.set_a_inputs(this->ir.fields.opcode);
    opcode_cmp.bed_0100011.set_a_inputs(this->ir.fields.opcode);
    opcode_cmp.bed_0010011.set_a_inputs(this->ir.fields.opcode);
    opcode_cmp.bed_0110011.set_a_inputs(this->ir.fields.opcode);
    opcode_cmp.bed_0001111.set_a_inputs(this->ir.fields.opcode);

    funct_3_cmp.bed_000.set_a_inputs(this->ir.fields.funct_3);
    funct_3_cmp.bed_001.set_a_inputs(this->ir.fields.funct_3);
    funct_3_cmp.bed_010.set_a_inputs(this->ir.fields.funct_3);
    funct_3_cmp.bed_011.set_a_inputs(this->ir.fields.funct_3);
    funct_3_cmp.bed_100.set_a_inputs(this->ir.fields.funct_3);
    funct_3_cmp.bed_101.set_a_inputs(this->ir.fields.funct_3);
    funct_3_cmp.bed_110.set_a_inputs(this->ir.fields.funct_3);
    funct_3_cmp.bed_111.set_a_inputs(this->ir.fields.funct_3);

    funct_7_cmp.bed_0100000.set_a_inputs(this->ir.fields.funct_7);
    funct_7_cmp.bed_0000000.set_a_inputs(this->ir.fields.funct_7);

    bed_ecall.set_a_inputs(this->ir.register_);
    bed_ebreak.set_a_inputs(this->ir.register_);

    {
        Gate_t op_bed_0110111 = opcode_cmp.bed_0110111.get_output();
        Gate_t op_bed_0010111 = opcode_cmp.bed_0010111.get_output();
        Gate_t op_bed_1101111 = opcode_cmp.bed_1101111.get_output();
        Gate_t op_bed_1100111 = opcode_cmp.bed_1100111.get_output();
        Gate_t op_bed_1100011 = opcode_cmp.bed_1100011.get_output();
        Gate_t op_bed_0000011 = opcode_cmp.bed_0000011.get_output();
        Gate_t op_bed_0100011 = opcode_cmp.bed_0100011.get_output();
        Gate_t op_bed_0010011 = opcode_cmp.bed_0010011.get_output(); // ADDI
        Gate_t op_bed_0110011 = opcode_cmp.bed_0110011.get_output();
        Gate_t op_bed_0001111 = opcode_cmp.bed_0001111.get_output();

        Gate_t f3_bed_000 = funct_3_cmp.bed_000.get_output();
        Gate_t f3_bed_001 = funct_3_cmp.bed_001.get_output();
        Gate_t f3_bed_010 = funct_3_cmp.bed_010.get_output();
        Gate_t f3_bed_011 = funct_3_cmp.bed_011.get_output();
        Gate_t f3_bed_100 = funct_3_cmp.bed_100.get_output();
        Gate_t f3_bed_101 = funct_3_cmp.bed_101.get_output();
        Gate_t f3_bed_110 = funct_3_cmp.bed_110.get_output();
        Gate_t f3_bed_111 = funct_3_cmp.bed_111.get_output();

        Gate_t f7_bed_0000000 = funct_7_cmp.bed_0000000.get_output();
        Gate_t f7_bed_0100000 = funct_7_cmp.bed_0100000.get_output();


        this->inst.LUI.add_input(op_bed_0110111); this->instruction_bit_value.at(0).add_input( this->inst.LUI );

        this->inst.AUIPC.add_input(op_bed_0010111); this->instruction_bit_value.at(1).add_input( this->inst.AUIPC );

        this->inst.JAL.add_input(op_bed_1101111); this->instruction_bit_value.at(2).add_input( this->inst.JAL );

        this->inst.JALR.add_input(And_( op_bed_1100111, f3_bed_000)); this->instruction_bit_value.at(3).add_input( this->inst.JALR );
        this->inst.BEQ.add_input(And_(  op_bed_1100011, f3_bed_000)); this->instruction_bit_value.at(4).add_input( this->inst.BEQ );
        this->inst.BNE.add_input(And_(  op_bed_1100011, f3_bed_001)); this->instruction_bit_value.at(5).add_input( this->inst.BNE );
        this->inst.BLT.add_input(And_(  op_bed_1100011, f3_bed_100)); this->instruction_bit_value.at(6).add_input( this->inst.BLT );
        this->inst.BGE.add_input(And_(  op_bed_1100011, f3_bed_101)); this->instruction_bit_value.at(7).add_input( this->inst.BGE );
        this->inst.BLTU.add_input(And_( op_bed_1100011, f3_bed_110)); this->instruction_bit_value.at(8).add_input( this->inst.BLTU );
        this->inst.BGEU.add_input(And_( op_bed_1100011, f3_bed_111)); this->instruction_bit_value.at(9).add_input( this->inst.BGEU );

        this->inst.LB.add_input(And_(  op_bed_0000011, f3_bed_000)); this->instruction_bit_value.at(10).add_input( this->inst.LB );
        this->inst.LH.add_input(And_(  op_bed_0000011, f3_bed_001)); this->instruction_bit_value.at(11).add_input( this->inst.LH );
        this->inst.LW.add_input(And_(  op_bed_0000011, f3_bed_010)); this->instruction_bit_value.at(12).add_input( this->inst.LW );
        this->inst.LBU.add_input(And_( op_bed_0000011, f3_bed_100)); this->instruction_bit_value.at(13).add_input( this->inst.LBU );
        this->inst.LHU.add_input(And_( op_bed_0000011, f3_bed_101)); this->instruction_bit_value.at(14).add_input( this->inst.LHU );

        this->inst.SB.add_input(And_( op_bed_0100011, f3_bed_000)); this->instruction_bit_value.at(15).add_input( this->inst.SB );
        this->inst.SH.add_input(And_( op_bed_0100011, f3_bed_001)); this->instruction_bit_value.at(16).add_input( this->inst.SH );
        this->inst.SW.add_input(And_( op_bed_0100011, f3_bed_010)); this->instruction_bit_value.at(17).add_input( this->inst.SW );

        this->inst.ADDI.add_input(And_(  op_bed_0010011, f3_bed_000)); this->instruction_bit_value.at(18).add_input( this->inst.ADDI );
        this->inst.SLTI.add_input(And_(  op_bed_0010011, f3_bed_010)); this->instruction_bit_value.at(19).add_input( this->inst.SLTI );
        this->inst.SLTIU.add_input(And_( op_bed_0010011, f3_bed_011)); this->instruction_bit_value.at(20).add_input( this->inst.SLTIU );
        this->inst.XORI.add_input(And_(  op_bed_0010011, f3_bed_100)); this->instruction_bit_value.at(21).add_input( this->inst.XORI );
        this->inst.ORI.add_input(And_(   op_bed_0010011, f3_bed_110)); this->instruction_bit_value.at(22).add_input( this->inst.ORI );
        this->inst.ANDI.add_input(And_(  op_bed_0010011, f3_bed_111)); this->instruction_bit_value.at(23).add_input( this->inst.ANDI );

        this->inst.SLLI.add_input(And_( op_bed_0010011, f3_bed_001, f7_bed_0000000 )); this->instruction_bit_value.at(24).add_input( this->inst.SLLI );
        this->inst.SRLI.add_input(And_( op_bed_0010011, f3_bed_101, f7_bed_0000000 )); this->instruction_bit_value.at(25).add_input( this->inst.SRLI );
        this->inst.SRAI.add_input(And_( op_bed_0010011, f3_bed_101, f7_bed_0100000 )); this->instruction_bit_value.at(26).add_input( this->inst.SRAI );

        this->inst.ADD.add_input(And_( op_bed_0110011,  f3_bed_000, f7_bed_0000000)); this->instruction_bit_value.at(27).add_input( this->inst.ADD );
        this->inst.SUB.add_input(And_( op_bed_0110011,  f3_bed_000, f7_bed_0100000)); this->instruction_bit_value.at(28).add_input( this->inst.SUB );
        this->inst.SLL.add_input(And_( op_bed_0110011,  f3_bed_001, f7_bed_0000000)); this->instruction_bit_value.at(29).add_input( this->inst.SLL );
        this->inst.SLT.add_input(And_( op_bed_0110011,  f3_bed_010, f7_bed_0000000)); this->instruction_bit_value.at(30).add_input( this->inst.SLT );
        this->inst.SLTU.add_input(And_( op_bed_0110011, f3_bed_011, f7_bed_0000000)); this->instruction_bit_value.at(31).add_input( this->inst.SLTU );
        this->inst.XOR.add_input(And_( op_bed_0110011,  f3_bed_100, f7_bed_0000000)); this->instruction_bit_value.at(32).add_input( this->inst.XOR );
        this->inst.SRL.add_input(And_( op_bed_0110011,  f3_bed_101, f7_bed_0000000)); this->instruction_bit_value.at(33).add_input( this->inst.SRL );
        this->inst.SRA.add_input(And_( op_bed_0110011,  f3_bed_101, f7_bed_0100000)); this->instruction_bit_value.at(34).add_input( this->inst.SRA );
        this->inst.OR.add_input(And_( op_bed_0110011,   f3_bed_110, f7_bed_0000000)); this->instruction_bit_value.at(35).add_input( this->inst.OR );
        this->inst.AND.add_input(And_( op_bed_0110011,  f3_bed_111, f7_bed_0000000)); this->instruction_bit_value.at(36).add_input( this->inst.AND );

        this->inst.FENCE.add_input(And_(op_bed_0001111, f3_bed_000)); this->instruction_bit_value.at(37).add_input( this->inst.FENCE );

        this->inst.ECALL.add_input( this->bed_ecall ); this->instruction_bit_value.at(38).add_input( this->inst.ECALL );

        this->inst.EBREAK.add_input( this->bed_ebreak ); this->instruction_bit_value.at(39).add_input( this->inst.EBREAK );
    }
    
    this->immediates.set_input_bus(this->ir.register_);
    this->immediates.set_select(RV32I_Imm::_11_0, 
            Or_(this->inst.JALR, this->inst.LB, this->inst.LH, this->inst.LW, this->inst.LBU, this->inst.LHU, 
                this->inst.ADDI, this->inst.SLTI, this->inst.SLTIU, this->inst.XORI, this->inst.ORI, this->inst.ANDI ));
    this->immediates.set_select(RV32I_Imm::_11_5,
            Or_(this->inst.SB, this->inst.SH, this->inst.SW));
    this->immediates.set_select(RV32I_Imm::_12_10_5,
            Or_(this->inst.BEQ, this->inst.BNE, this->inst.BLT, this->inst.BGE, this->inst.BLTU, this->inst.BGEU));
    this->immediates.set_select(RV32I_Imm::_4_0,
            Or_(this->inst.SB, this->inst.SH, this->inst.SW));
    this->immediates.set_select(RV32I_Imm::_4_1_11,
            Or_(this->inst.BEQ, this->inst.BNE, this->inst.BLT, this->inst.BGE, this->inst.BLTU, this->inst.BGEU));
    this->immediates.set_select(RV32I_Imm::_31_12,
            Or_(this->inst.LUI, this->inst.AUIPC));
    this->immediates.set_select(RV32I_Imm::_20_10_1_11_19_12,
            Or_(this->inst.JAL));


    VectorConnect_(this->decode_data.RS1,   VectorSlice_(this->ir.register_, 15, 19+1));
    VectorConnect_(this->decode_data.RS2,   VectorSlice_(this->ir.register_, 20, 24+1));
    VectorConnect_(this->decode_data.RD,    VectorSlice_(this->ir.register_,  7, 11+1));
    VectorConnect_(this->decode_data.shamt, VectorSlice_(this->ir.register_, 20, 24+1));
    VectorConnect_(this->decode_data.imm32, this->immediates.bus);
    VectorConnect_(this->decode_data.inst,  this->instruction_bit_value);
}

void RISCV_Decoder_t::set_data_in(std::vector<Gate_t> input) {
    assert(input.size() == 32);
    VectorConnect_(this->ir.register_, input);
}

void RISCV_Decoder_t::set_clk(Gate_t clk) {
    this->ir.set_clk(clk);
}

void RISCV_Decoder_t::Test(void) {

    auto test_bit_set_only = [](RISCV_Decoder_t& decoder, size_t idx) {
        assert(idx >= 0 && idx <= 39);

        //for(size_t i : range(0, 40))
        //    std::cout << decoder.instruction_bit_value.at(i).get_output();
        //std::cout << " : " << instruction_name_lut[idx] << " (" << idx << ")" << std::endl;

        for(size_t i = 0; i < 40; i++) {
            if(i == idx) {
                assert(decoder.instruction_bit_value.at(i).get_output() == 1);
            }
            else {
                assert(decoder.instruction_bit_value.at(i).get_output() == 0);
            }
        }

    };

    auto rbs = [](size_t width) -> std::string {
        std::string s;
        for(size_t i = 0UL; i < width; i++) {
            s.push_back("01"[rand() & 0x01]);
            //s.push_back('1');
        }
        return s;
    };

    auto ts = []() -> size_t {
        timeval tv;
        gettimeofday(&tv, NULL);
        return (tv.tv_sec * 1000000UL + tv.tv_usec) / 1000UL; // return milliseconds
    };

    srand(time(NULL));

    logic_init();

    RISCV_Decoder_t decoder_0;

    decoder_0.Verify();

    SignalVector_t sig(32);
    Signal_t       clk;

    decoder_0.set_data_in(sig);
    decoder_0.set_clk(clk);

    // call this AFTER all connections are made and before
    // the first call to logic_gate_simulate()
    logic_gate_eval_forwards();

    clk.set(0);
    logic_gate_simulate();

    size_t start_time = ts();

    const size_t NUM_TEST_RUNS = 1ul;

    for(size_t i = 0UL; i < NUM_TEST_RUNS; i++) {

        // need to test at least one of every instruction
        std::vector<std::tuple<std::string, int>> test_cases = {
            { rbs(20) + rbs(5) + "0110111", 0 }, // LUI
            { rbs(20) + rbs(5) + "0010111", 1 }, // AUIPC
            { rbs(20) + rbs(5) + "1101111", 2 }, // JAL

            { rbs(12) + rbs(5) + "000" + rbs(5) + "1100111", 3 }, // JALR

            { rbs(7) + rbs(5) + rbs(5) + "000" + rbs(5) + "1100011", 4 }, // BEQ
            { rbs(7) + rbs(5) + rbs(5) + "001" + rbs(5) + "1100011", 5 }, // BNE
            { rbs(7) + rbs(5) + rbs(5) + "100" + rbs(5) + "1100011", 6 }, // BLT
            { rbs(7) + rbs(5) + rbs(5) + "101" + rbs(5) + "1100011", 7 }, // BGE
            { rbs(7) + rbs(5) + rbs(5) + "110" + rbs(5) + "1100011", 8 }, // BLTU
            { rbs(7) + rbs(5) + rbs(5) + "111" + rbs(5) + "1100011", 9 }, // BGEU

            { rbs(12) + rbs(5) + "000" + rbs(5) + "0000011", 10 }, // LB
            { rbs(12) + rbs(5) + "001" + rbs(5) + "0000011", 11 }, // LH
            { rbs(12) + rbs(5) + "010" + rbs(5) + "0000011", 12 }, // LW
            { rbs(12) + rbs(5) + "100" + rbs(5) + "0000011", 13 }, // LBU
            { rbs(12) + rbs(5) + "101" + rbs(5) + "0000011", 14 }, // LHU

            { rbs(7) + rbs(5) + rbs(5) + "000" + rbs(5) + "0100011", 15 }, // SB
            { rbs(7) + rbs(5) + rbs(5) + "001" + rbs(5) + "0100011", 16 }, // SH
            { rbs(7) + rbs(5) + rbs(5) + "010" + rbs(5) + "0100011", 17 }, // SW

            { rbs(12) + rbs(5) + "000" + rbs(5) + "0010011", 18 }, // ADDI
            { rbs(12) + rbs(5) + "010" + rbs(5) + "0010011", 19 }, // SLTI
            { rbs(12) + rbs(5) + "011" + rbs(5) + "0010011", 20 }, // SLTIU
            { rbs(12) + rbs(5) + "100" + rbs(5) + "0010011", 21 }, // XORI
            { rbs(12) + rbs(5) + "110" + rbs(5) + "0010011", 22 }, // ORI
            { rbs(12) + rbs(5) + "111" + rbs(5) + "0010011", 23 }, // ANDI

            { "0000000" + rbs(5) + rbs(5) + "001" + rbs(5) + "0010011", 24 }, // SLLI
            { "0000000" + rbs(5) + rbs(5) + "101" + rbs(5) + "0010011", 25 }, // SRLI
            { "0100000" + rbs(5) + rbs(5) + "101" + rbs(5) + "0010011", 26 }, // SRAI

            { "0000000" + rbs(5) + rbs(5) + "000" + rbs(5) + "0110011", 27 }, // ADD
            { "0100000" + rbs(5) + rbs(5) + "000" + rbs(5) + "0110011", 28 }, // SUB
            { "0000000" + rbs(5) + rbs(5) + "001" + rbs(5) + "0110011", 29 }, // SLL
            { "0000000" + rbs(5) + rbs(5) + "010" + rbs(5) + "0110011", 30 }, // SLT
            { "0000000" + rbs(5) + rbs(5) + "011" + rbs(5) + "0110011", 31 }, // SLTU
            { "0000000" + rbs(5) + rbs(5) + "100" + rbs(5) + "0110011", 32 }, // XOR
            { "0000000" + rbs(5) + rbs(5) + "101" + rbs(5) + "0110011", 33 }, // SRL
            { "0100000" + rbs(5) + rbs(5) + "101" + rbs(5) + "0110011", 34 }, // SRA
            { "0000000" + rbs(5) + rbs(5) + "110" + rbs(5) + "0110011", 35 }, // OR
            { "0000000" + rbs(5) + rbs(5) + "111" + rbs(5) + "0110011", 36 }, // AND

            { rbs(12) + rbs(5) + "000" + rbs(5) + "0001111", 37 }, // FENCE

            { "00000000000000000000000001110011", 38 }, // ECALL
            { "00000000000100000000000001110011", 39 }, // EBREAK

        };

        for(auto& tu : test_cases) {
            sig.set_value_r(std::get<0>(tu));
            
            // simulate rising edge of clock
            clk.set(1);
            logic_gate_simulate();
            clk.set(0);
            logic_gate_simulate();

            //print_opcode_cmp_data(decoder);
            test_bit_set_only(decoder_0, (size_t)std::get<1>(tu));
        }
    }

    size_t end_time = ts();
    std::cout << "total time: " << (end_time - start_time) << " ms" << std::endl;

    logic_allocator_debug_info();
    logic_clean();
}

void RISCV_Decoder_t::Verify(void) {
    this->immediates.Verify();



}
