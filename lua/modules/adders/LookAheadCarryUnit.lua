--
-- 4-bit carry lookahead unit
-- can be used to nest arbitrarily deep adders
--

require("simload");
require("gate");


LookAheadCarryUnit = {}

LookAheadCarryUnit.new = function(c0, g0, p0, g1, p1, g2, p2, g3, p3)

    assert(c0, 'LookAheadCarryUnit : c0 is nil');
    assert(g0, 'LookAheadCarryUnit : g0 is nil');
    assert(p0, 'LookAheadCarryUnit : p0 is nil');
    assert(g1, 'LookAheadCarryUnit : g1 is nil');
    assert(p1, 'LookAheadCarryUnit : p1 is nil');
    assert(g2, 'LookAheadCarryUnit : g2 is nil');
    assert(p2, 'LookAheadCarryUnit : p2 is nil');
    assert(g3, 'LookAheadCarryUnit : g3 is nil');
    assert(p3, 'LookAheadCarryUnit : p3 is nil');

    local self = {};

    self.c0 = c0;

    self.c1 =
        Gate.OR(
            g0,
            Gate.AND( p0, c0 ));

    self.c2 =
        Gate.OR(
            g1,
            Gate.AND( g0, p1 ),
            Gate.AND( c0, p0, p1 ));

    self.c3 =
        Gate.OR(
            g2,
            Gate.AND( g1, p2 ),
            Gate.AND( g0, p1, p2 ),
            Gate.AND( c0, p0, p1, p2 ));

    self.c4 =
        Gate.OR(
            g3,
            Gate.AND( g2, p3 ),
            Gate.AND( g1, p2, p3 ),
            Gate.AND( g0, p1, p2, p3 ),
            Gate.AND( c0, p0, p1, p2, p3 ));

    self.PG_and = Gate.AND( p0, p1, p2, p3 );

    self.GG_or = 
        Gate.OR(
            g3,
            Gate.AND( g2, p3 ),
            Gate.AND( g1, p3, p2 ),
            Gate.AND( g0, p3, p2, p1 ));

    return self;
end
