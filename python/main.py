from __future__ import print_function

import os
import sys

from Gate import *
from src.BitVector import *
from lib.modules.simpleadder import *

A = SIGNAL()
B = SIGNAL()
Ci = SIGNAL()

Sum = None
Co  = None

# create a 4-bit adder
adders = []
for i in range(0, 4):
    adders.append(simpleadder())

adders[1].set_carry_in(adders[0].get_carry_out())
adders[2].set_carry_in(adders[1].get_carry_out())
adders[3].set_carry_in(adders[2].get_carry_out())

adder = simpleadder()

A.set_value(0)
B.set_value(0)
Ci.set_value(0)

adder.set_a(A)
adder.set_b(B)
adder.set_carry_in(Ci)

Sum = adder.get_sum()
Co  = adder.get_carry_out()

for i in range(0, 8):
    A.set_value(  (i >> 0) & 1 )
    B.set_value(  (i >> 1) & 1 )
    Ci.set_value( (i >> 2) & 1 )

    hdl.logic_gate_simulate()


