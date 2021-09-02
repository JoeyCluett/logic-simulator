#include <lib/modules/adders/CarryLookaheadAdder.h>

#include <lib/cpp-api/gate-wrapper.h>
#include <lib/cpp-api/throw-exception.h>
#include <stdexcept>

CarryLookaheadAdder_t::CarryLookaheadAdder_t(size_t width) 
    :
        cla_unit(width),
        adders(width),
        Cin_forward(FORWARD()),
        Cout_forward(FORWARD()) {

    this->adder_width = width;

    // initialize gates and forwards
    for(size_t i = 0UL; i < width; i++) {
        this->A_forwards.push_back(FORWARD());
        this->B_forwards.push_back(FORWARD());
        this->Sum_forwards.push_back(FORWARD());

        this->adders.at(i).set_a(this->A_forwards.at(i));
        this->adders.at(i).set_b(this->B_forwards.at(i));
    }

    this->cla_unit.set_cin_input(this->Cin_forward);

    std::vector<Gate_t> p;
    std::vector<Gate_t> g;

    // collate propagates and generates
    for(size_t i = 0UL; i < width; i++) {
        p.push_back(this->adders.at(i).get_propagate());
        g.push_back(this->adders.at(i).get_generate());
    }

    // pass propagate and generate inputs to CLA unit
    this->cla_unit.set_generate_inputs(g);
    this->cla_unit.set_propagate_inputs(p);

    std::vector<Gate_t> carry_outs = this->cla_unit.get_carry_outputs();

    // carry outs go back to the full adders
    for(size_t idx = 0UL; idx < width; idx++)
        this->adders.at(idx).set_ci(carry_outs.at(idx));

    // sum result needs to go to Sum_forwards
    for(size_t idx = 0UL; idx < width; idx++)
        this->Sum_forwards.at(idx).add_input(this->adders.at(idx).get_sum());

    // final carry out
    this->Cout_forward.add_input(carry_outs.back());
}

std::vector<Gate_t> CarryLookaheadAdder_t::get_sum(void) {
    std::vector<Gate_t> s;

    for(auto& a : this->adders)
        s.push_back(a.get_sum());

    return s;
}


void CarryLookaheadAdder_t::set_a_inputs(std::vector<Gate_t> A) {
    if(A.size() != this->adder_width)
        throw std::runtime_error(
                "CarryLookaheadAdder_t::set_a_inputs : width "
                "of input vector must match adder width");

    for(size_t i = 0UL; i < this->adder_width; i++) {
        this->A_forwards.at(i).add_input(A.at(i));
    }
}

void CarryLookaheadAdder_t::set_b_inputs(std::vector<Gate_t> B) {
    if(B.size() != this->adder_width)
        throw std::runtime_error(
                "CarryLookaheadAdder_t::set_b_inputs : width "
                "of input vector must match adder width");

    for(size_t i = 0UL; i < this->adder_width; i++) {
        this->B_forwards.at(i).add_input(B.at(i));
    }
}

void CarryLookaheadAdder_t::set_carry_in(Gate_t g) {
    this->Cin_forward.add_input(g);
}

Gate_t CarryLookaheadAdder_t::get_carry_out(void) {
    return this->Cout_forward;
}

void CarryLookaheadAdder_t::VerifyInputs(void) {

    try {
        this->cla_unit.VerifyInputs();
    }
    catch(std::runtime_error& re) {
        THROW_RE_p(re.what(), "error in CarryLookaheadUnit_t");
    }

    for(size_t i = 0UL; i < this->adder_width; i++) {
        try { this->adders.at(i).VerifyInputs(); }
        catch(std::runtime_error& re) {
            THROW_RE_p(re.what(), "error in this->adders[" + std::to_string(i) + "]");
        }

        try { this->A_forwards.at(i).has_inputs(); }
        catch(std::runtime_error& re) {
            THROW_RE_p(re.what(), "this->A_forwards[" + std::to_string(i) + "] is N.C.");
        }

        try { this->B_forwards.at(i).has_inputs(); }
        catch(std::runtime_error& re) {
            THROW_RE_p(re.what(), "this->B_forwards[" + std::to_string(i) + "] is N.C.");
        }

        try { this->Sum_forwards.at(i).has_inputs(); }
        catch(std::runtime_error& re) {
            THROW_RE_p(re.what(), "this->Sum_forwards[" + std::to_string(i) + "] is N.C.");
        }

    }

    if(this->Cin_forward.has_inputs() == false)
        THROW_RE("this->Cin_forward is N.C.");

    if(this->Cout_forward.has_inputs() == false)
        THROW_RE("this->Cin_forward is N.C.");

}
