
require("simload");
require("gate");

require("Adder_CLA_1Bit");
require("LookAheadCarryUnit");

Adder_CLA_4Bit = {};

Adder_CLA_4Bit.new = function()
    -- print("Adder_CLA_4Bit.new");

    local self = {}

    self.adders = {}
    for i=0,3 do self.adders[i] = Adder_CLA_1Bit.new(); end

    -- ==========================================
    -- internal logic
    -- ==========================================

    self.c0_ = Gate.OR();

    local c0 = self.c0_;

    local g0 = self.adders[0].get_G();
    local p0 = self.adders[0].get_P();
    local g1 = self.adders[1].get_G();
    local p1 = self.adders[1].get_P();
    local g2 = self.adders[2].get_G();
    local p2 = self.adders[2].get_P();
    local g3 = self.adders[3].get_G();
    local p3 = self.adders[3].get_P();

    local clu = LookAheadCarryUnit.new(c0, g0, p0, g1, p1, g2, p2, g3, p3);

    -- set the carry in of the adder slices
    self.adders[0].set_CarryIn(clu.c0);
    self.adders[1].set_CarryIn(clu.c1);
    self.adders[2].set_CarryIn(clu.c2);
    self.adders[3].set_CarryIn(clu.c3);

    self.c4_ = clu.c4;

    self.PG_and = clu.PG_and;
    self.GG_or  = clu.GG_or;

    -- ==========================================
    -- interface methods
    -- ==========================================

    self.set_A = function(idx, gate) self.adders[idx].set_A(gate); end
    self.set_B = function(idx, gate) self.adders[idx].set_B(gate); end

    self.get_Sum = function(idx) return self.adders[idx].get_Sum(); end
    self.get_PG  = function()    return self.PG_and; end
    self.get_GG  = function()    return self.GG_or;  end

    self.set_CarryIn  = function(gate) self.c0_.add_input(gate); end
    self.get_CarryOut = function() return self.c4_; end
    self.get_BitWidth = function() return 4; end

    return self;
end
