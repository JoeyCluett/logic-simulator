
require("simload");
require("gate");

HalfAdder = {}

HalfAdder.new = function()

    print("HalfAdder.new");

    local self = {};

    self.g_and = Gate.AND();
    self.g_xor = Gate.XOR();

    self.set_A = function(gate)
        self.g_and.add_input(gate);
        self.g_xor.add_input(gate);
    end

    self.set_B = function(gate)
        self.g_and.add_input(gate);
        self.g_xor.add_input(gate);
    end

    self.get_Sum   = function() return self.g_xor; end
    self.get_Carry = function() return self.g_and; end

    return self;
end

