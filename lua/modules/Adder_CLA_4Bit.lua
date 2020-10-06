
-- optimize this later. this implementation doesnt need a 
-- carry out bit. PG and GG implicitly handle that through 
-- an external LCU

require("simload");
require("gate");

require("Adder_CLA_1Bit");

Adder_CLA_4Bit = {};

Adder_CLA_4Bit.new = function()
    print("Adder_CLA_4Bit.new");

    local self = {}

    self.adders = {}
    for i=0,3 do self.adders[i] = Adder_CLA_1Bit.new(); end

    self.c0_ = Gate.OR();

    -- ==========================================
    -- internal logic
    -- ==========================================

    local c0 = self.c0_;
    local g0 = self.adders[0].get_G();
    local p0 = self.adders[0].get_P();
    local g1 = self.adders[1].get_G();
    local p1 = self.adders[1].get_P();
    local g2 = self.adders[2].get_G();
    local p2 = self.adders[2].get_P();
    local g3 = self.adders[3].get_G();
    local p3 = self.adders[3].get_P();

    local c1 =
        Gate.OR(
            g0,
            Gate.AND( c0, p0 ));

    local c2 = 
        Gate.OR(
            g1,
            Gate.AND( g0, p1 ),
            Gate.AND( c0, p0, p1 ));

    local c3 = 
        Gate.OR(
            g2,
            Gate.AND( g1, p2 ),
            Gate.AND( g0, p1, p2 ),
            Gate.AND( c0, p0, p1, p2 ));

    local c4 = 
        Gate.OR(
            g3,
            Gate.AND( g2, p3 ),
            Gate.AND( g1, p2, p3 ),
            Gate.AND( g0, p1, p2, p3 ),
            Gate.AND( c0, p0, p1, p2, p3 ));

    -- set the carry in of the adder slices
    self.adders[0].set_CarryIn(c0);
    self.adders[1].set_CarryIn(c1);
    self.adders[2].set_CarryIn(c2);
    self.adders[3].set_CarryIn(c3);

    self.c4_ = c4;

    self.PG_and = Gate.AND( p0, p1, p2, p3 );

    self.GG_or = 
        Gate.OR(
            g3,
            Gate.AND( g2, p3 ),
            Gate.AND( g1, p3, p2 ),
            Gate.AND( g0, p3, p2, p1 ));

    -- ==========================================
    -- interface methods
    -- ==========================================

    self.set_A = function(idx, gate) self.adders[idx].set_A(gate); end
    self.set_B = function(idx, gate) self.adders[idx].set_B(gate); end

    self.get_Sum = function(idx) return self.adders[idx].get_Sum(); end
    self.get_PG  = function() return self.PG_and; end
    self.get_GG  = function() return self.GG_or; end

    self.get_CarryOut = function() return self.c4_; end
    self.set_CarryIn  = function(gate) self.c0_.add_input(gate); end

    self.get_BitWidth = function() return 4; end

    return self;
end
