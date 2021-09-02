#include <lib/modules/adders/FullAdderCLA.h>

#include <lib/cpp-api/throw-exception.h>

FullAdder_CLA_t::FullAdder_CLA_t(void) 
    :
        and_lower(AND()),
        xor_upper(XOR()),
        xor_lower(XOR()),
        
        forward_A(FORWARD()),
        forward_B(FORWARD()),
        forward_Cin(FORWARD()) {

    this->xor_upper.add_input(this->xor_lower);
    this->xor_upper.add_input(this->forward_Cin);

    this->and_lower.add_input(this->forward_A);
    this->and_lower.add_input(this->forward_B);

    this->xor_lower.add_input(this->forward_A);
    this->xor_lower.add_input(this->forward_B);
}

void FullAdder_CLA_t::set_a(Gate_t a) {
    this->forward_A.add_input(a);
}

void FullAdder_CLA_t::set_b(Gate_t b) {
    this->forward_B.add_input(b);
}

void FullAdder_CLA_t::set_ci(Gate_t ci) {
    this->forward_Cin.add_input(ci);
}

Gate_t FullAdder_CLA_t::get_sum(void) {
    return this->xor_upper;
}

Gate_t FullAdder_CLA_t::get_generate(void) {
    return this->and_lower;
}

Gate_t FullAdder_CLA_t::get_propagate(void) {
    return this->xor_lower;
}

void FullAdder_CLA_t::VerifyInputs(void) {

    if(this->forward_A.has_inputs() == false) {
        THROW_RE("A input has no inputs");
    }

    if(this->forward_B.has_inputs() == false) {
        THROW_RE("B input has no inputs");
    }

    if(this->forward_Cin.has_inputs() == false) {
        THROW_RE("Cin input has no inputs");
    }

}

