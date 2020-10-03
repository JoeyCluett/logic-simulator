-- this file places what would otherwise be package functions into the global namespace

local _sim = require("logic_wrapper");
print("moving 'logic_wrapper' package functions into global namespace...");

allocator_logic_init             = _sim.allocator_logic_init;
allocator_logic_allocator_ngates = _sim.allocator_logic_allocator_ngates;
allocator_logic_gate_alloc       = _sim.allocator_logic_gate_alloc;
allocator_logic_input_alloc      = _sim.allocator_logic_input_alloc;

utility_AND  = _sim.utility_AND;
utility_NAND = _sim.utility_NAND;
utility_OR   = _sim.utility_OR;
utility_NOR  = _sim.utility_NOR;
utility_XOR  = _sim.utility_XOR;
utility_XNOR = _sim.utility_XNOR;

utility_FLIPFLOP = _sim.utility_FLIPFLOP;
utility_CONSTANT = _sim.utility_CONSTANT;
utility_SIGNAL   = _sim.utility_SIGNAL;
utility_logic_gate_add_input = _sim.utility_logic_gate_add_input;
utility_logic_gate_simulate  = _sim.utility_logic_gate_simulate;
utility_logic_gate_simulate_callback  = _sim.utility_logic_gate_simulate_callback;
utility_logic_gate_flipflop_set_clock = _sim.utility_logic_gate_flipflop_set_clock;
utility_logic_gate_flipflop_set_data  = _sim.utility_logic_gate_flipflop_set_data;

logic_gate_new  = _sim.logic_gate_new;
logic_gate_init = _sim.logic_gate_init;
logic_gate_signal_set = _sim.logic_gate_signal_set;
logic_gate_get_output = _sim.logic_gate_get_output;

get_constant_zero = _sim.get_constant_zero;
get_constant_one  = _sim.get_constant_one;
