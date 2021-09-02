#include <lib/modules/TwosCmpl2SignMag.h>

#include <stdexcept>

TwosCmpl2SignMag_t::TwosCmpl2SignMag_t(size_t input_width)
    :
        input_inverters(VectorInit_(input_width, NOR)),
        input_forwards(VectorInit_(input_width, FORWARD)),
        sign_output(FORWARD()),
        mux(input_width),
        adder(input_width) {

    // sign bit is forwarded directly to the output
    this->sign_output.add_input(this->input_forwards.back());

    // adder always adds 1
    this->adder.set_carry_in(ONE);

    for(size_t i = 0UL; i < input_width; i++) {
        this->input_inverters.at(i).add_input(this->input_forwards.at(i));

        this->adder.B_forwards.at(i).add_input(ZERO);
        this->adder.A_forwards.at(i).add_input(this->input_inverters.at(i));
    }

    this->mux.set_one_input(this->adder.get_sum());
    this->mux.set_zero_input(this->input_forwards);
    this->mux.set_select_input(this->input_forwards.back());
}

void TwosCmpl2SignMag_t::set_input(std::vector<Gate_t> v) {
    if(v.size() != this->input_forwards.size())
        throw std::runtime_error("TwosCmpl2SignMag_t::set_input : input vector size must match module width");

    for(size_t i = 0UL; i < v.size(); i++) {
        this->input_forwards.at(i).add_input(v.at(i));
    }
}

auto TwosCmpl2SignMag_t::get_output_magnitude(void) -> std::vector<Gate_t> {
    return this->mux.get_output();
}

Gate_t TwosCmpl2SignMag_t::get_output_sign(void) {
    return this->sign_output;
}
