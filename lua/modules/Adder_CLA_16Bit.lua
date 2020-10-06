
require("simload");
require("gate");
require("Adder_CLA_4Bit");

Adder_CLA_16Bit = {};

Adder_CLA_16Bit.new = function()

    local self = {};

    self.adders = {};
    for i=0,3 do 
        self.adders[i] = Adder_CLA_4Bit.new(); 
    end

    self.c0_  = Gate.OR();

    local c0  = self.c0_;

    local p0  = self.adders[0].get_PG();
    local g0  = self.adders[0].get_GG();
    local p4  = self.adders[1].get_PG();
    local g4  = self.adders[1].get_GG();
    local p8  = self.adders[2].get_PG();
    local g8  = self.adders[2].get_GG();
    local p12 = self.adders[3].get_PG();
    local g12 = self.adders[3].get_GG();

    local c4 = 
        Gate.OR(
            g0,
            Gate.AND( c0, p0 ));

    local c8 = 
        Gate.OR(
            g4,
            Gate.AND( g0, p4 ),
            Gate.AND( c0, p0, p4 ));

    local c12 =
        Gate.OR(
            g8,
            Gate.AND( g4, p8 ),
            Gate.AND( g0, p4, p8 ),
            Gate.AND( c0, p0, p4, p8 ));

    self.c16_ =
        Gate.OR(
            g12,
            Gate.AND( g8, p12 ),
            Gate.AND( g4, p8, p12 ),
            Gate.AND( g0, p4, p8, p12 ),
            Gate.AND( c0, p0, p4, p8, p12 ));

    -- connect carries to 4-bit adders
    self.adders[0].set_CarryIn(self.c0_);
    self.adders[1].set_CarryIn(c4);
    self.adders[2].set_CarryIn(c8);
    self.adders[3].set_CarryIn(c12);

    -- ==========================================
    -- interface methods
    -- ==========================================

    self.get_CarryOut = function() return self.c16_; end
    self.set_CarryIn  = function(gate) self.c0_.add_input(gate); end

    self.get_Sum = function(idx)
        if     idx >= 0  and idx <= 3  then return self.adders[0].get_Sum(idx - 0);
        elseif idx >= 4  and idx <= 7  then return self.adders[1].get_Sum(idx - 4);
        elseif idx >= 8  and idx <= 11 then return self.adders[2].get_Sum(idx - 8);
        elseif idx >= 12 and idx <= 15 then return self.adders[3].get_Sum(idx - 12);
        else
            error('Adder_CLA_16Bit.get_Sum : index \'' .. idx .. '\' is invalid');
        end
    end

    self.set_A = function(idx, gate)
        if     idx >= 0  and idx <= 3  then self.adders[0].set_A(idx - 0, gate);
        elseif idx >= 4  and idx <= 7  then self.adders[1].set_A(idx - 4, gate);
        elseif idx >= 8  and idx <= 11 then self.adders[2].set_A(idx - 8, gate);
        elseif idx >= 12 and idx <= 15 then self.adders[3].set_A(idx - 12, gate);
        else
            error('Adder_CLA_16Bit.set_A : index \'' .. idx .. '\' is invalid');
        end
    end

    self.set_B = function(idx, gate)
        if     idx >= 0  and idx <= 3  then self.adders[0].set_B(idx - 0, gate);
        elseif idx >= 4  and idx <= 7  then self.adders[1].set_B(idx - 4, gate);
        elseif idx >= 8  and idx <= 11 then self.adders[2].set_B(idx - 8, gate);
        elseif idx >= 12 and idx <= 15 then self.adders[3].set_B(idx - 12, gate);
        else
            error('Adder_CLA_16Bit.set_B : index \'' .. idx .. '\' is invalid');
        end
    end

    self.get_BitWidth = function() return 16; end

    return self;
end

