#include <lib/modules/homebrew/OverflowDetector.h>

OverflowDetector_t::OverflowDetector_t(void) 
        : x(XOR()), xn(XNOR()), a0(AND()), 
        a1(AND()), a2(AND()), out(OR()) {

    this->add.set(FORWARD());
    this->sub.set(FORWARD());
    this->sign_a.set(FORWARD());
    this->sign_b.set(FORWARD());
    this->sign_f.set(FORWARD());
    this->sign_a_inv.set(NOR());
    this->sign_b_inv.set(NOR());
    this->sign_f_inv.set(NOR());
    this->sign_a_inv.add_input(this->sign_a);
    this->sign_b_inv.add_input(this->sign_b);
    this->sign_f_inv.add_input(this->sign_f);

    this->x.add_input(this->sign_a);
    this->x.add_input(this->sign_b);

    this->xn.add_input(this->sign_b);
    this->xn.add_input(this->sign_f);

    this->a0.add_input(this->x);
    this->a0.add_input(this->xn);
    this->a0.add_input(this->sub);

    this->a1.add_input(this->sign_a);
    this->a1.add_input(this->sign_b);
    this->a1.add_input(this->sign_f_inv);
    this->a1.add_input(this->add);    

    this->a2.add_input(this->sign_a_inv);
    this->a2.add_input(this->sign_b_inv);
    this->a2.add_input(this->sign_f);
    this->a2.add_input(this->add);

    this->out.add_input(this->a0);
    this->out.add_input(this->a1);
    this->out.add_input(this->a2);
}

Gate_t OverflowDetector_t::get_output(void) {
    return this->out;
}

void OverflowDetector_t::set_signal_add(Gate_t gate) {
    this->add.add_input(gate);
}

void OverflowDetector_t::set_signal_sub(Gate_t gate) {
    this->sub.add_input(gate);
}

void OverflowDetector_t::set_signal_sign_a(Gate_t gate) {
    this->sign_a.add_input(gate);
}

void OverflowDetector_t::set_signal_sign_b(Gate_t gate) {
    this->sign_b.add_input(gate);
}

void OverflowDetector_t::set_signal_sign_f(Gate_t gate) {
    this->sign_f.add_input(gate);
}

