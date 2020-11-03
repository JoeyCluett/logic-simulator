from __future__ import print_function

from Gate import *
from src.BitVector import *
from lib.modules.shiftregister import *

sr = sipo_shiftregister(4)

data = SIGNAL()
clock = SIGNAL()

data.set_value(0)
clock.set_value(0)

sr.set_data_in(data)
sr.set_clock(clock)

for i in range(0, 4):
    clock.set_value(0)
    hdl.logic_gate_simulate()
    clock.set_value(1)
    hdl.logic_gate_simulate()
clock.set_value(0)
hdl.logic_gate_simulate()

data.set_value(1)

for i in range(0, 4):
    clock.set_value(0)
    hdl.logic_gate_simulate()
    clock.set_value(1)
    hdl.logic_gate_simulate()

    for j in range(0, 4):
        print(sr.get_output_value(), end='')
    print()

