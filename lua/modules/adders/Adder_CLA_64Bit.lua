
require("simload");
require("gate");
require("Adder_CLA_16Bit");
require("LookAheadCarryUnit");

Adder_CLA_64Bit = {};

Adder_CLA_64Bit.new = function(carry_in_gate)

    local self = {};

    if carry_in_gate ~= nil 
    then self.c0_ = carry_in_gate;
    else self.c0_ = Gate.OR();
    end

    self.adders = {};
    for i=0,3 do 
        if i == 0 
        then self.adders[i] = Adder_CLA_16Bit.new(self.c0_); 
        else self.adders[i] = Adder_CLA_16Bit.new(); 
        end
    end

    -- ==========================================
    -- internal logic
    -- ==========================================

    local c0  = self.c0_;
    local p0  = self.adders[0].get_PG();
    local g0  = self.adders[0].get_GG();
    local p4  = self.adders[1].get_PG();
    local g4  = self.adders[1].get_GG();
    local p8  = self.adders[2].get_PG();
    local g8  = self.adders[2].get_GG();
    local p12 = self.adders[3].get_PG();
    local g12 = self.adders[3].get_GG();

    local clu = LookAheadCarryUnit.new(c0, g0, p0, g4, p4, g8, p8, g12, p12);

    -- connect carries to 4-bit adders
    self.adders[0].set_CarryIn(clu.c0);
    self.adders[1].set_CarryIn(clu.c1);
    self.adders[2].set_CarryIn(clu.c2);
    self.adders[3].set_CarryIn(clu.c3);

    self.c16_   = clu.c4;
    self.PG_and = clu.PG_and;
    self.GG_or  = clu.GG_or;

    -- ==========================================
    -- interface methods
    -- ==========================================

    self.get_PG = function() return self.PG_and; end
    self.get_GG = function() return self.GG_or;  end

    self.get_CarryOut = function() return self.c16_; end
    self.set_CarryIn  = function(gate) self.c0_.add_input(gate); end

    self.get_Sum = function(idx)
        if     idx >= 0  and idx <= 15 then return self.adders[0].get_Sum(idx - 0);
        elseif idx >= 16 and idx <= 31 then return self.adders[1].get_Sum(idx - 16);
        elseif idx >= 32 and idx <= 47 then return self.adders[2].get_Sum(idx - 32);
        elseif idx >= 48 and idx <= 63 then return self.adders[3].get_Sum(idx - 48);
        else
            error('Adder_CLA_64Bit.get_Sum : index \'' .. idx .. '\' is invalid');
        end
    end

    self.set_A = function(idx, gate)
        if     idx >= 0  and idx <= 15 then self.adders[0].set_A(idx - 0,  gate);
        elseif idx >= 16 and idx <= 31 then self.adders[1].set_A(idx - 16, gate);
        elseif idx >= 32 and idx <= 47 then self.adders[2].set_A(idx - 32, gate);
        elseif idx >= 48 and idx <= 63 then self.adders[3].set_A(idx - 48, gate);
        else
            error('Adder_CLA_64Bit.set_A : index \'' .. idx .. '\' is invalid');
        end
    end

    self.set_B = function(idx, gate)
        if     idx >= 0  and idx <= 15 then self.adders[0].set_B(idx - 0,  gate);
        elseif idx >= 16 and idx <= 31 then self.adders[1].set_B(idx - 16, gate);
        elseif idx >= 32 and idx <= 47 then self.adders[2].set_B(idx - 32, gate);
        elseif idx >= 48 and idx <= 63 then self.adders[3].set_B(idx - 48, gate);
        else
            error('Adder_CLA_64Bit.set_B : index \'' .. idx .. '\' is invalid');
        end
    end

    self.get_BitWidth = function() return 64; end

    return self;
end

