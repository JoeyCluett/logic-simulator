#include <lib/modules/adders/CarryLookaheadUnit.h>

#include <lib/logic.h>
#include <lib/cpp-api/throw-exception.h>
#include <lib/cpp-api/gate-wrapper.h>
#include <lib/cpp-api/gate-varargs.h>
#include <lib/cpp-api/range.h>

#include <stdexcept>

/* example

c1 = 
         g0  + // T1
    c0 * p0    // T0

c2 = 
              g1  + // T2  
         g0 * p1  + // T1
    c0 * p0 * p1    // T0

c3 = 
                   g2  + // T3 
              g1 * p2  + // T2
         g0 * p1 * p2  + // T1
    c0 * p0 * p1 * p2    // T0

c4 = 
                        g3  + // T4
                   g2 * p3  + // T3 
              g1 * p2 * p3  + // T2
         g0 * p1 * p2 * p3  + // T1
    c0 * p0 * p1 * p2 * p3    // T0

*/

CarryLookaheadUnit_t::CarryLookaheadUnit_t(size_t width) 
    //:
    //    PG(FORWARD()),
    //    GG(FORWARD()) 
        {

    this->cla_width = width;

    for(size_t i = 0; i < width; i++) {
        this->g.push_back(FORWARD());
        this->p.push_back(FORWARD());
        this->c.push_back(FORWARD());
    }

    // c needs N+1 entries (first one is the carry-in)
    this->c.push_back(FORWARD());

/*
    {
        Gate_t g_and(AND());
        for(size_t i = 0UL; i < width; i++)
            g_and.add_input(this->p.at(i));
        this->PG.add_input(g_and);

        // TODO: GG
        this->GG.add_input(g_and);
    }
*/

    for(size_t C = 1UL; C <= width; C++) {
        Gate_t g_or(OR());

        for(size_t T = 0UL; T <= C; T++) {

            if(T == 0) {
                Gate_t g_and(AND());
                g_and.add_input(this->c.at(0));
                for(size_t p_idx = 0UL; p_idx < C; p_idx++)
                    g_and.add_input(this->p.at(p_idx));
                g_or.add_input(g_and);
            }
            else if(T == C) {
                g_or.add_input(this->g.at(C-1));
            }
            else {

                Gate_t g_and(AND());
                const size_t g_idx = T-1UL;
                g_and.add_input(this->g.at(g_idx));

                for(size_t p_idx = T; p_idx < C; p_idx++)
                    g_and.add_input(this->p.at(p_idx));

                g_or.add_input(g_and);                
            }
        }

        this->c.at(C).add_input(g_or);
    }

}

void CarryLookaheadUnit_t::set_cin_input(Gate_t g) {
    this->c.at(0).add_input(g);
}

void CarryLookaheadUnit_t::set_propagate_inputs(std::vector<Gate_t> p_) {
    if(p_.size() != this->cla_width) {
        throw std::runtime_error("CarryLookaheadUnit_t::set_propagate_inputs : input must match bit width of lookahead unit");
    }

    for(size_t i = 0UL; i < this->cla_width; i++)
        this->p.at(i).add_input(p_.at(i));
}

void CarryLookaheadUnit_t::set_generate_inputs(std::vector<Gate_t> g_) {
    if(g_.size() != this->cla_width) {
        throw std::runtime_error("CarryLookaheadUnit_t::set_generate_inputs : input must match bit width of lookahead unit");
    }

    for(size_t i = 0UL; i < this->cla_width; i++)
        this->g.at(i).add_input(g_.at(i));
}

std::vector<Gate_t> CarryLookaheadUnit_t::get_carry_outputs(void) {
    return this->c;
}

/*
Gate_t CarryLookaheadUnit_t::get_propagate(void) {
    return this->PG;
}

Gate_t CarryLookaheadUnit_t::get_generate(void) {
    return this->GG;
}
*/

void CarryLookaheadUnit_t::VerifyInputs(void) {

    for(size_t i = 0UL; i < this->cla_width; i++) {
        if(this->p.at(i).has_inputs() == false) {
            THROW_RE("propagate input [" + std::to_string(i) + "] is N.C.");
        }

        if(this->g.at(i).has_inputs() == false) {
            THROW_RE("generate input [" + std::to_string(i) + "] is N.C.");
        }

        if(this->c.at(0).has_inputs() == false) {
            THROW_RE("carry input is N.C.");
        }
    }
}

