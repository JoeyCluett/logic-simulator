-- this file functions as a wrapper over the C logic_gate_t 
-- ptrs that are exposed via the Lua-C API

--reveals many Lua-C functions
--require("logic_wrapper");

require("simload");

print("defining 'Gate' package...");

Gate = {
    type_and      = 0,
    type_nand     = 1,
    type_or       = 2,
    type_nor      = 3,
    type_xor      = 4,
    type_xnor     = 5,
    type_flipflop = 6,
    type_none     = 7, -- used by pre-allocated gates until they are ready for use
    type_constant = 8,
    type_signal   = 9  -- input type
}

-- simulation function evaluates state of current gates
Gate.simulate = function()
    return utility_logic_gate_simulate();
end


Gate.new = function(type, initial_value, ...)

    local self = {};
   
    -- standard gate types
    if type == Gate.type_and then       self.ptr = utility_AND();
    elseif type == Gate.type_nand then  self.ptr = utility_NAND();
    elseif type == Gate.type_or then    self.ptr = utility_OR();
    elseif type == Gate.type_nor then   self.ptr = utility_NOR();
    elseif type == Gate.type_xor then   self.ptr = utility_XOR();
    elseif type == Gate.type_xnor then  self.ptr = utility_XNOR();

    -- special gate types
    elseif type == Gate.type_flipflop then  self.ptr = utility_FLIPFLOP();
    elseif type == Gate.type_none then      self.ptr = utility_NONE();
    elseif type == Gate.type_constant then

        if initial_value == 0 then
            self.ptr = get_constant_zero();
        elseif initial_value == 1 then
            self.ptr = get_constant_one();
        else
            print('undefined constant ' .. initial_value);
            self.ptr = utility_CONSTANT(initial_value);
        end

    elseif type == Gate.type_signal then    self.ptr = utility_SIGNAL();
    else
        error("error in Gate.new() : invalid gate type specified");
    end

    self.add_input = function(...)     
        for _,v in ipairs({...}) do
            utility_logic_gate_add_input(self.ptr, v.ptr); 
        end
    end

    self.add_inv_input = function(gate)
        -- create intermediate NOR gate
        local g_not = Gate.NOR();
        g_not.add_input(gate);
        utility_logic_gate_add_input(self.ptr, g_not.ptr);
    end

    self.set_signal_value   = function(value) logic_gate_signal_set(self.ptr, value); end
    self.set_flipflop_clock = function(gate) utility_logic_gate_flipflop_set_clock(self.ptr, gate.ptr); end
    self.set_flipflop_data  = function(gate) utility_logic_gate_flipflop_set_data(self.ptr, gate.ptr); end
    self.get_value          = function() return logic_gate_get_output(self.ptr); end

    -- input_list may contain a list of input gates
    for _, v in ipairs({...}) do
        self.add_input(v);
    end

    return self;
end

-- standard gate wrappers
Gate.AND  = function(...) return Gate.new(Gate.type_and,  nil, ...) end
Gate.NAND = function(...) return Gate.new(Gate.type_nand, nil, ...) end
Gate.OR   = function(...) return Gate.new(Gate.type_or,   nil, ...) end
Gate.NOR  = function(...) return Gate.new(Gate.type_nor,  nil, ...) end
Gate.XOR  = function(...) return Gate.new(Gate.type_xor,  nil, ...) end
Gate.XNOR = function(...) return Gate.new(Gate.type_xnor, nil, ...) end

-- special gate wrappers
Gate.FLIPFLOP = function()      return Gate.new(Gate.type_flipflop, nil) end
Gate.NONE     = function()      return Gate.new(Gate.type_none,     nil) end
Gate.CONSTANT = function(value) return Gate.new(Gate.type_constant, value) end
Gate.SIGNAL   = function()      return Gate.new(Gate.type_signal,   nil) end

Gate.ONE  = Gate.CONSTANT(1)
Gate.ZERO = Gate.CONSTANT(0)

-- more convenient than calling .add_input multiple times
Gate.add_input_list = function(gate, inputlist)
    for k, v in ipairs(inputlist) do
        gate.add_input(v);
    end
end


