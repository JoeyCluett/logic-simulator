
-- optimize this later. this implementation doesnt need a 
-- carry out bit. PG and GG implicitly handle that through 
-- an external LCU

require("simload");
require("gate");

require("Adder_74283"); -- 4 bit adder w/o P,G

Adder_CLA_4Bit = {};

Adder_CLA_4Bit.new = function()
    print("Adder_CLA_4Bit.new");

    self = {}

    -- doing this instead of using metatables because 
    -- functionality needs to be modified in many cases
    self.adder = Adder_74283.new();
    self.g     = {};
    self.p     = {};

    for i=1,4 do
        self.g[i-1] = Gate.AND();
        self.p[i-1] = Gate.OR();
    end

    self.PG_and = Gate.AND(); -- group propagate
    self.GG_or  = Gate.OR();  -- group generate

    -- ==========================================
    -- interface methods
    -- ==========================================

    self.set_A = function(idx, gate)
        self.adder.set_A(idx, gate);
        self.g[idx-1].add_input(gate);
        self.p[idx-1].add_input(gate);
    end

    self.set_B = function(idx, gate)
        self.adder.set_B(idx, gate);
        self.g[idx-1].add_input(gate);
        self.p[idx-1].add_input(gate);
    end

    self.get_Sum = function(idx) return self.adder.get_Sum(idx); end

    self.get_PG  = function() return self.PG_and; end
    self.get_GG  = function() return self.GG_or; end

    self.get_CarryOut = function() return self.adder.CO_nor; end
    self.set_CarryIn = function(gate) self.adder.CI_not.add_input(gate); end

    -- ==========================================
    -- internal logic
    -- ==========================================

    local and_0 = Gate.AND();
    local and_1 = Gate.AND();
    local and_2 = Gate.AND();
    local and_3 = Gate.AND();

    and_1.add_input( self.p[3], self.g[2] );
    and_2.add_input( self.p[3], self.p[2], self.g[1] );
    and_3.add_input( self.p[3], self.p[2], self.p[1], self.g[0] );

    self.PG_and.add_input(
        self.p[3], self.p[0], 
        self.p[2], self.p[1]);

    self.GG_or.add_input(self.g[3], and_0, and_1, and_2);

    return self;
end
