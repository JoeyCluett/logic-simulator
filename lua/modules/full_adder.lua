
require("simload");
require("gate");

require("half_adder");

FullAdder = {}

FullAdder.new = function()

    print("FullAdder.new");

    local self = {};

    self.lower = HalfAdder.new();
    self.upper = HalfAdder.new();
    self.upper.set_A(self.lower.get_Sum())

    self.g_or = Gate.OR();
    self.g_or.add_input(self.lower.get_Carry());
    self.g_or.add_input(self.upper.get_Carry());

    self.set_A = function(gate) self.lower.set_A(gate); end
    self.set_B = function(gate) self.lower.set_A(gate); end

    self.set_CarryIn  = function(gate) self.upper.set_A(gate); end
    self.get_Sum      = function(gate) return self.upper.get_Sum(); end
    self.get_CarryOut = function(gate) return self.g_or; end
    
    return self;

end

