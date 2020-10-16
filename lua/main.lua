package.cpath = package.cpath .. ';./lib/?.so';
package.path  = package.path  .. ';./lib/?.lua';
package.path  = package.path  .. ';./modules/?.lua';
package.path  = package.path  .. ';./modules/adders/?.lua';

-- these things are required to use the C logic simulation backend
require("simload");
require("gate");

-- debug information automatically handles print redirects. 
-- also exposes static GlobalDebug object
require("globaldebug")
GlobalDebug.set_debug_value(true);

-- initialize the memory allocator. used to efficiently allocate logic gate 
-- structs internally. Lua never actually allocates the structures used 
-- internally by the C backend
allocator_logic_init(); -- THIS IS NOT OPTIONAL

-- import module and connect inputs
require("BCD_Generator_NBit");

set_input_vector_value = function(bit_width, input_arr, value)
    for i=0,(bit_width-1) do
        input_arr[i].set_signal_value((value >> i) & 0x01);
    end
end

bcd = BCD_Generator_NBit.new(8);



-- allocate module and input signals. capture outputs
--adder    = Adder_CLA_4Bit.new();
--adder    = Adder_CLA_16Bit.new();
adder    = Adder_CLA_64Bit.new();
ci       = Gate.SIGNAL();
inputs_a = {};
inputs_b = {};
outputs  = {};

adder.set_CarryIn(ci);

for i=0,(adder.get_BitWidth() - 1) do
    inputs_a[i] = Gate.SIGNAL();
    inputs_b[i] = Gate.SIGNAL();
    outputs[i]  = adder.get_Sum(i);

    adder.set_A(i, inputs_a[i]);
    adder.set_B(i, inputs_b[i]);
end

outputs[adder.get_BitWidth()] = adder.get_CarryOut();
ci.set_signal_value(0);

for i=0,63 do
    inputs_a[i].set_signal_value(1);
    inputs_b[i].set_signal_value(0);
end

ncycles = Gate.simulate();
print('cycles: ' .. ncycles);

inputs_b[0].set_signal_value(1);

ncycles = Gate.simulate();
print('cycles: ' .. ncycles);


