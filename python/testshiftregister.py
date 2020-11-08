from __future__ import print_function

from Gate import *
from BitVector import *
from lib.modules.shiftregister import *

sr = sipo_shiftregister(20)

data = SIGNAL()
clock = SIGNAL()

data.set_output_value(0)
clock.set_output_value(0)

sr.set_data_in(data)
sr.set_clock(clock)

for i in range(0, 4):
    clock.set_output_value(0)
    hdl.logic_gate_simulate()
    clock.set_output_value(1)
    hdl.logic_gate_simulate()
clock.set_output_value(0)
hdl.logic_gate_simulate()

data.set_output_value(1)

for i in range(0, 4):
    clock.set_output_value(0)
    hdl.logic_gate_simulate()
    clock.set_output_value(1)
    hdl.logic_gate_simulate()

    for j in range(0, sr.length):
        print(sr.get_output_value(j), end='')
    print()

data.set_output_value(0)

for i in range(0, 4):
    clock.set_output_value(0)
    hdl.logic_gate_simulate()
    clock.set_output_value(1)
    hdl.logic_gate_simulate()

    for j in range(0, sr.length):
        print(sr.get_output_value(j), end='')
    print()

data.set_output_value(1)

for i in range(0, 4):
    clock.set_output_value(0)
    hdl.logic_gate_simulate()
    clock.set_output_value(1)
    hdl.logic_gate_simulate()

    for j in range(0, sr.length):
        print(sr.get_output_value(j), end='')
    print()

data.set_output_value(0)

for i in range(0, 4):
    clock.set_output_value(0)
    hdl.logic_gate_simulate()
    clock.set_output_value(1)
    hdl.logic_gate_simulate()

    for j in range(0, sr.length):
        print(sr.get_output_value(j), end='')
    print()
