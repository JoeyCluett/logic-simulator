
require("simload");
require("gate");

require("Adder_CLA_1Bit");

Adder_CLA_8Bit = {};

Adder_CLA_8Bit.new = function()

    local self = {};

    self.adders = {};
    for i=0,7 do self.adders[i] = Adder_CLA_1Bit.new(); end

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
    local g4 = self.adders[4].get_G();
    local p4 = self.adders[4].get_P();
    local g5 = self.adders[5].get_G();
    local p5 = self.adders[5].get_P();
    local g6 = self.adders[6].get_G();
    local p6 = self.adders[6].get_P();
    local g7 = self.adders[7].get_G();
    local p7 = self.adders[7].get_P();

    local c1 = Gate.OR( 
        g0,
        Gate.AND( p0, c0 ));

    local c2 = Gate.OR( 
        g1,
        Gate.AND( p1, g0 ),
        Gate.AND( p1, p0, c0 ));
    
    local c3 = Gate.OR( 
        g2,
        Gate.AND( p2, g1 ),
        Gate.AND( p2, p1, g0 ),
        Gate.AND( p2, p1, p0, c0 ));
    
    local c4 = Gate.OR( 
        g3,
        Gate.AND( p3, g2 ),
        Gate.AND( p3, p2, g1 ),
        Gate.AND( p3, p2, p1, g0 ),
        Gate.AND( p3, p2, p1, p0, c0 ));
    
    local c5 = Gate.OR(
        g4,
        Gate.AND( p4, g3 ),
        Gate.AND( p4, p3, g2 ),
        Gate.AND( p4, p3, p2, g1 ),
        Gate.AND( p4, p3, p2, p1, g0 ),
        Gate.AND( p4, p3, p2, p1, p0, c0 ));
    
    local c6 = Gate.OR(
        g5,
        Gate.AND( p5, g4 ),
        Gate.AND( p5, p4, g3 ),
        Gate.AND( p5, p4, p3, g2 ),
        Gate.AND( p5, p4, p3, p2, g1 ),
        Gate.AND( p5, p4, p3, p2, p1, g0 ),
        Gate.AND( p5, p4, p3, p2, p1, p0, c0 ));
    
    local c7 = Gate.OR( 
        g6,
        Gate.AND( p6, g5 ),
        Gate.AND( p6, p5, g4 ),
        Gate.AND( p6, p5, p4, g3 ),
        Gate.AND( p6, p5, p4, p3, g2 ),
        Gate.AND( p6, p5, p4, p3, p2, g1 ),
        Gate.AND( p6, p5, p4, p3, p2, p1, g0 ),
        Gate.AND( p6, p5, p4, p3, p2, p1, p0, c0 ));
    
    local c8 = Gate.OR(
        g7,
        Gate.AND( p7, g6 ),
        Gate.AND( p7, p6, g5 ),
        Gate.AND( p7, p6, p5, g4 ),
        Gate.AND( p7, p6, p5, p4, g3 ),
        Gate.AND( p7, p6, p5, p4, p3, g2 ),
        Gate.AND( p7, p6, p5, p4, p3, p2, g1 ),
        Gate.AND( p7, p6, p5, p4, p3, p2, p1, g0 ),
        Gate.AND( p7, p6, p5, p4, p3, p2, p1, p0, c0 ));

    return self;
end
