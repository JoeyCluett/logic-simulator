#include <lib/modules/adders/HalfAdder.h>

#include <lib/logic.h>

HalfAdder_t::HalfAdder_t(void) 
        : sum(XOR()), carry(AND()),
        forward_a(FORWARD()),
        forward_b(FORWARD()) {

    this->sum.add_input(this->forward_a);
    this->sum.add_input(this->forward_b);
    this->carry.add_input(this->forward_a);
    this->carry.add_input(this->forward_b);
}

void HalfAdder_t::set_a(Gate_t gate) {
    this->forward_a.add_input(gate);
}

void HalfAdder_t::set_b(Gate_t gate) {
    this->forward_b.add_input(gate);
}

Gate_t HalfAdder_t::get_carry(void) {
    return this->carry;
}

Gate_t HalfAdder_t::get_sum(void) {
    return this->sum;
}

