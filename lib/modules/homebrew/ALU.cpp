#include <lib/modules/homebrew/ALU.h>
#include <lib/cpp-api/range.h>

#include <vector>

ALU_t::ALU_t(void) 
        : 
            logic_unit(16), adder(16), 
            mux(16), register_a(16), 
            register_accumulator(16), 
            register_flags(4),
            mux_select(FORWARD()),
            internal_flag_carry(FORWARD()),
            internal_flag_sign(FORWARD()),
            internal_flag_overflow(FORWARD()),
            internal_flag_zero(FORWARD()),
            fn_add(FORWARD()), fn_sub(FORWARD()),
            fn_and(FORWARD()), fn_nand(FORWARD()),
            fn_or(FORWARD()),  fn_nor(FORWARD()),
            fn_xor(FORWARD()), fn_xnor(FORWARD()) {

    // set input bus as forward type
    for(int i : range(0, 16))
        this->input_forward[i].set(FORWARD());

    // connect logic unit and adder to output multiplexer
    this->mux.set_zero_input(this->logic_unit.get_output_vector());
    this->mux.set_one_input(this->adder.get_sum());

    // connect A register to inputs on logic unit and adder
    this->logic_unit.set_a(this->register_a.get_output());
    this->adder.set_a_inputs(this->register_a.get_output());

    // connect input bus to inputs on logic unit and adder   
    this->logic_unit.set_b({ this->input_forward, this->input_forward+16 });
    this->adder.set_b_inputs({ this->input_forward, this->input_forward+16 });

    // connect input bus to A register
    this->register_a.set_data({ this->input_forward, this->input_forward+16 });

    // set inputs for zero detector
    this->zero_detector.set_input(this->mux.get_output());

    // set all internal flag signals (before flags register)
    this->internal_flag_carry.add_input(this->adder.get_carry_out());             // flags[0]
    this->internal_flag_sign.add_input(this->mux.get_output().at(15));            // flags[1]
    this->internal_flag_overflow.add_input(this->overflow_detector.get_output()); // flags[2]
    this->internal_flag_zero.add_input(this->zero_detector.get_output());         // flags[3]

    this->register_flags.set_data({
            this->internal_flag_carry,
            this->internal_flag_sign,
            this->internal_flag_overflow,
            this->internal_flag_zero
        });

    // set all input signals for overflow detector
    this->overflow_detector.set_signal_sign_a(this->register_a.get_output(15));
    this->overflow_detector.set_signal_sign_b(this->input_forward[15]);
    this->overflow_detector.set_signal_sign_f(this->mux.get_output().at(15));
    this->overflow_detector.set_signal_add(this->fn_add);
    this->overflow_detector.set_signal_sub(this->fn_sub);

}

std::vector<Gate_t> ALU_t::get_output_flags(void) {
    return this->register_flags.get_output();
}
