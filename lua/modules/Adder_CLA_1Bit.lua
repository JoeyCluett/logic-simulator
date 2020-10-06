
require("simload");
require("gate");

Adder_CLA_1Bit = {};

Adder_CLA_1Bit.new = function(has_carry_out)
    print("Adder_CLA_1Bit.new");

    local self = {};

    self.lo_xor = Gate.XOR();
    self.lo_and = Gate.AND();
    self.hi_xor = Gate.XOR(self.lo_xor);

    -- ==========================================
    -- interface methods
    -- ==========================================

    self.get_P   = function() return self.lo_xor; end
    self.get_G   = function() return self.lo_and; end
    self.get_Sum = function() return self.hi_xor; end
    self.set_CarryIn = function(gate) self.hi_xor.add_input(gate); end
    
    self.set_A = function(gate) 
        self.lo_xor.add_input(gate); 
        self.lo_and.add_input(gate);
    end

    self.set_B = function(gate) 
        self.lo_xor.add_input(gate); 
        self.lo_and.add_input(gate);
    end

    return self;
end
