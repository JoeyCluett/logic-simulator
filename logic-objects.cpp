#include "logic-objects.h"

LogicGate::LogicGate(void) {
    this->ptr = NULL;
}

// initialize with existing logic_gate_t ptr
LogicGate::LogicGate(logic_gate_t* ptr) {
    this->ptr = ptr;
}

// destructor wont do anything
LogicGate::~LogicGate() {
    return;
}

// explicit cast to underlying type
LogicGate::operator logic_gate_t*() {
    return this->ptr;
}

logic_gate_t* LogicGate::operator&(LogicGate& rhs) {
    return And(this->ptr, rhs.ptr);
}

