#include <lib/modules/adders/FullAdder.h>

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

FullAdder_t::FullAdder_t(void) 
        : carry_out(OR()),
        cin_forward(FORWARD()) {
    
    this->upper.set_a(this->lower.get_sum());
    this->upper.set_b(this->cin_forward);
    this->carry_out.add_input(this->upper.get_carry());
    this->carry_out.add_input(this->lower.get_carry());
}

void FullAdder_t::set_a(Gate_t a) {
    this->lower.set_a(a);
}

void FullAdder_t::set_b(Gate_t b) {
    this->lower.set_b(b);
}

void FullAdder_t::set_ci(Gate_t ci) {
    this->cin_forward.add_input(ci);
}

Gate_t FullAdder_t::get_co(void) {
    return this->carry_out;
}

Gate_t FullAdder_t::get_sum(void) {
    return this->upper.get_sum();
}

Gate_t FullAdder_t::get_cla_generate(void) {
    return this->lower.get_carry();
}

Gate_t FullAdder_t::get_cla_propagate(void) {
    return this->lower.get_sum();
}
