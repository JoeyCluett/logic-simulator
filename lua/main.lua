package.cpath = package.cpath .. ';./lib/?.so';
package.path  = package.path  .. ';./lib/?.lua';
package.path  = package.path  .. ';./modules/?.lua';

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
require("full_adder");
require("Adder_CLA_4Bit");
require("Adder_CLA_16Bit");

set_input_vector_value = function(bit_width, input_arr, value)
    for i=0,(bit_width-1) do
        input_arr[i].set_signal_value((value >> i) & 0x01);
    end
end

verify_results = function(bit_width, a_arr, b_arr, r_arr, offset)
    a_val = 0;
    b_val = 0;
    r_val = 0;

    for i=0,(bit_width-1) do
        a_val = a_val | (a_arr[i].get_value() << i);
        b_val = b_val | (b_arr[i].get_value() << i);
        r_val = r_val | (r_arr[i].get_value() << i);
    end

    r_val = r_val | (r_arr[bit_width].get_value() << bit_width);

    -- all three numbers have been built up   
    --print(a_val .. " + " .. b_val .. " + " .. offset .. " = " .. r_val);
    if (a_val + b_val + offset) ~= r_val then

        print('error:');
        io.write('  ');
        for i=0,(bit_width-1) do io.write(a_arr[(bit_width-1) - i].get_value()); end
        io.write('\n  ')
        for i=0,(bit_width-1) do io.write(b_arr[(bit_width-1) - i].get_value()); end
        io.write('\n ');
        for i=0,bit_width do io.write(r_arr[(bit_width)-i].get_value()); end
        print('');

        error(a_val .. " + " .. b_val .. " + " .. offset .. " does not equal " .. r_val);
    end
end

-- allocate module and input signals. capture outputs
--adder    = Adder_CLA_4Bit.new();
adder    = Adder_CLA_16Bit.new();
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

for a=0,65535 do
    for b=0,1 do

        set_input_vector_value(adder.get_BitWidth(), inputs_a, a);
        set_input_vector_value(adder.get_BitWidth(), inputs_b, (1 << b));

        ncycles = Gate.simulate();

        verify_results(adder.get_BitWidth(), inputs_a, inputs_b, outputs, 0);

    end
end

ci.set_signal_value(1);

for a=0,1 do
    for b=0,65535 do

        set_input_vector_value(adder.get_BitWidth(), inputs_a, (1 << a));
        set_input_vector_value(adder.get_BitWidth(), inputs_b, b);

        ncycles = Gate.simulate();

        verify_results(adder.get_BitWidth(), inputs_a, inputs_b, outputs, 1);

    end
end

ci.set_signal_value(0);
set_input_vector_value(adder.get_BitWidth(), inputs_a, 65535);
set_input_vector_value(adder.get_BitWidth(), inputs_b, 0);
ncycles = Gate.simulate();

set_input_vector_value(adder.get_BitWidth(), inputs_b, 1);
ncycles = Gate.simulate();
print('cycles: ' .. ncycles);
