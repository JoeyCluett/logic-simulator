#pragma once

#include "logic-gates.h"
#include "logic-gate-allocator.h"
#include "logic-gate-utility.h"

#include <iostream>

using namespace std;

struct LogicGate {
public:
    logic_gate_t* ptr;

    LogicGate(void);

    // initialize with existing logic_gate_t ptr
    LogicGate(logic_gate_t* ptr);

    // destructor wont do anything
    ~LogicGate();

    // explicit cast to underlying type
    operator logic_gate_t*();

    // generate bitwise operations
    logic_gate_t* operator&(LogicGate& rhs);
    logic_gate_t* operator|(LogicGate& rhs);
    logic_gate_t* operator^(LogicGate& rhs);
};

struct AndGate : public LogicGate {

    AndGate(void)

};

template<typename Dummy>
logic_gate_t* And(void) {
    return AND();
}

template<typename Dummy>
logic_gate_t* And(logic_gate_t* gptr) {
    auto* g = AND();
    logic_gate_add_input(g, gptr);
    return g;
}

template<typename Dummy>
logic_gate_t* And(logic_gate_t* lhs, logic_gate_t* rhs) {
    if(lhs->gate_type == logic_gate_and) {
        logic_gate_add_input(lhs, rhs);
        return lhs;
    }
    else if(rhs->gate_type == logic_gate_and) {
        logic_gate_add_input(rhs, lhs);
        return rhs;
    }
    else {
        auto* tmp_gate = AND();
        logic_gate_add_input(tmp_gate, lhs);
        logic_gate_add_input(tmp_gate, rhs);
        return tmp_gate;
    }
}

template<typename...GateTypes>
logic_gate_t* And(logic_gate_t* gptr, GateTypes... gs) {
    return And(gptr, And(gs...));
}

/*

    auto g = And(And( A, B ), Or( C ));
    auto h = And(Or( C ), And( A, B ));

    auto g = !A & B | C;
    Or(And(Not(A), B), C);
    auto x = !A & B | B & !A;

    auto x = Or(And(Not(A), B), And(A, Not(B)));

*/

