#include <lib/cpp-api/gate-wrapper.h>
#include <lib/logic.h>

#include <assert.h>

#include <iostream>

std::vector<logic_gate_t*> VectorCvt_(std::vector<Gate_t> v) {
    std::vector<logic_gate_t*> v_;
    for(auto g : v)
        v_.push_back(g.internal_gate);
    return v_;
}

std::vector<Gate_t> VectorInit_(const std::string& s) {
    std::vector<Gate_t> v;

    for(char c : s) {
        switch(c) {
            case '0':
                v.push_back(ZERO);
                break;
            case '1':
                v.push_back(ONE);
                break;
            default:
                throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " : invalid char '" + std::to_string(c) + "'");
        }
    }

    return v;
}

std::vector<Gate_t> VectorInit_(int width, size_t constant) {
    return VectorInit_((size_t)width, (size_t)constant);
}

std::vector<Gate_t> VectorInit_(size_t width, int constant) {
    return VectorInit_((size_t)width, (size_t)constant);
}

std::vector<Gate_t> VectorInit_(int width, int constant) {
    return VectorInit_((size_t)width, (size_t)constant);
}

std::vector<Gate_t> VectorInit_(size_t width, size_t constant) {
    std::vector<Gate_t> v;
    size_t tmp_const = constant;

    for(size_t i = 0UL; i < width; i++) {
        switch(constant & 0x01) {
            case 0:
                v.push_back(ZERO);
                break;
            case 1:
                v.push_back(ONE);
                break;
            default:
                throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " : unknown internal error");
        }

        constant = constant >> 1;
    }

    if(constant)
        throw std::runtime_error(
                std::string(__PRETTY_FUNCTION__) + " : constant given '" + 
                std::to_string(tmp_const) + "' is more than " + std::to_string(width) + 
                " bits wide");

    return v;
}

void VectorConnect_(std::vector<Gate_t> dst, std::vector<Gate_t> src) {
    for(size_t i = 0UL; i < dst.size(); i++) {
        if(dst.at(i).internal_gate->gate_type == logic_gate_flipflop) {
            dst.at(i).ff_set_data(src.at(i));
        }
        else {
            dst.at(i).add_input(src.at(i));
        }
    }
}

auto VectorSlice_(std::vector<Gate_t>& src, size_t start, size_t end) -> std::vector<Gate_t> {
    std::vector<Gate_t> v;
    for(size_t i = start; i < end; i++)
        v.push_back(src.at(i));
    return v;
}

Gate_t ForwardWrap_(Gate_t g) {
    Gate_t fwd(FORWARD());
    fwd.add_input(g);
    return fwd;
}

Gate_t operator&(Gate_t lhs, Gate_t rhs) {
    if(lhs.internal_gate->gate_type == logic_gate_and) {
        lhs.add_input(rhs);
        return lhs;
    }
    else {
        Gate_t andgate(AND());
        andgate.add_input(lhs);
        andgate.add_input(rhs);
        return andgate;
    }
}

Gate_t operator|(Gate_t lhs, Gate_t rhs) {
    if(lhs.internal_gate->gate_type == logic_gate_or) {
        lhs.add_input(rhs);
        return lhs;
    }
    else {
        Gate_t orgate(OR());
        orgate.add_input(lhs);
        orgate.add_input(rhs);
        return orgate;
    }
}

Gate_t operator^(Gate_t lhs, Gate_t rhs) {
    if(lhs.internal_gate->gate_type == logic_gate_xor) {
        lhs.add_input(rhs);
        return lhs;        
    }
    else {
        Gate_t xorgate(XOR());
        xorgate.add_input(lhs);
        xorgate.add_input(rhs);
        return xorgate;
    }
}

Gate_t operator~(Gate_t g) {
    switch(g.internal_gate->gate_type) {
        case logic_gate_and:
            g.internal_gate->gate_type = logic_gate_nand;
            break;
        case logic_gate_nand:
            g.internal_gate->gate_type = logic_gate_and;
            break;
        case logic_gate_or:
            g.internal_gate->gate_type = logic_gate_nor;
            break;
        case logic_gate_nor:
            g.internal_gate->gate_type = logic_gate_or;
            break;
        case logic_gate_xor:
            g.internal_gate->gate_type = logic_gate_xnor;
            break;
        case logic_gate_xnor:
            g.internal_gate->gate_type = logic_gate_xor;
            break;

        case logic_gate_flipflop:
        case logic_gate_constant:
        case logic_gate_signal:
        case logic_gate_forward:
        case logic_gate_buffer:
        case logic_gate_wire:
            {
                Gate_t g_(NOR());
                g_.add_input(g);
                return g_;
            }
            break;

        default:
            throw std::runtime_error(
                    "operator~(Gate_t) : invalid input gate type '" + 
                    std::to_string((int)g.internal_gate->gate_type) + "'");
    }

    return g;
}

Gate_t::Gate_t(void)
        : internal_gate(NULL) {
}

Gate_t::Gate_t(logic_gate_t* gate_ptr)
        : internal_gate(gate_ptr) {
}

Gate_t::operator logic_gate_ptr_t() {
    return this->internal_gate;
}

void Gate_t::set_output(int val) {
    logic_gate_signal_set(this->internal_gate, val);
}

int Gate_t::get_output(void) {
    return logic_gate_follow(this->internal_gate)->output_value;
}

void Gate_t::set(logic_gate_t* gate_ptr) {
    this->internal_gate = gate_ptr;
}

void Gate_t::add_input(Gate_t gate) {
    logic_gate_add_input(this->internal_gate, gate.internal_gate);
}

int Gate_t::num_inputs(void) {
    //
    // TODO
    //
    return 0;
}

void Gate_t::ff_set_clock(Gate_t gate) {
    logic_gate_flipflop_set_clock(this->internal_gate, gate.internal_gate);
}

void Gate_t::ff_set_data(Gate_t gate) {
    logic_gate_flipflop_set_data(this->internal_gate, gate.internal_gate);
}

void Gate_t::tristate_set_data(Gate_t gate) {
    logic_gate_tristate_set_data(this->internal_gate, gate.internal_gate);
}

void Gate_t::tristate_set_enable(Gate_t gate) {
    logic_gate_tristate_set_enable(this->internal_gate, gate.internal_gate);
}

bool Gate_t::has_inputs(void) {
    switch(this->internal_gate->gate_type) {
    case logic_gate_and      :
    case logic_gate_nand     :
    case logic_gate_or       :
    case logic_gate_nor      :
    case logic_gate_xor      :
    case logic_gate_xnor     :
        if(this->internal_gate->gate.inputs == NULL)
            return 0;
        return 1;

    case logic_gate_flipflop :
        if(
                this->internal_gate->flipflop.clk_input == NULL ||
                this->internal_gate->flipflop.data_input == NULL)
            return 0;
        return 1;

    case logic_gate_none     :
    case logic_gate_constant :
    case logic_gate_signal   :
        throw std::runtime_error(
                "Gate_t::has_inputs : invalid gate type '" + 
                std::to_string((int)this->internal_gate->gate_type) + "'");

    case logic_gate_forward  :
        if(this->internal_gate->_forward.input_gate == NULL)
            return 0;
        return 1;

    case logic_gate_buffer   :
        if(
                this->internal_gate->tristate_buffer.data_input == NULL ||
                this->internal_gate->tristate_buffer.select_input == NULL)
            return 0;
        return 1;

    case logic_gate_wire     :
        if(this->internal_gate->wire.input_list == NULL)
            return 0;
        return 1;

    case logic_gate_re_detect:
        throw std::runtime_error(
                "Gate_t::has_inputs : invalid gate type '" + 
                std::to_string((int)this->internal_gate->gate_type) + "'");

    default:
        throw std::runtime_error("Gate_t::has_inputs : unknown internal error");

    }
}
