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
require("Adder_74283");
require("Adder_CLA_4Bit");

set_input_vector_value = function(input_arr, value)
    for i=1,4 do
        input_arr[i].set_signal_value((value >> (i-1)) & 0x01);
    end
end

verify_results = function(a_arr, b_arr, r_arr, offset)
    a_val = 0;
    b_val = 0;
    r_val = 0;

    for i=1,4 do
        a_val = a_val | (a_arr[i].get_value() << (i-1));
        b_val = b_val | (b_arr[i].get_value() << (i-1));
        r_val = r_val | (r_arr[i].get_value() << (i-1));
    end

    -- r_val has one additional bit
    r_val = r_val | (r_arr[5].get_value() << 4);

    -- all three numbers have been built up   
    --print(a_val .. " + " .. b_val .. " + " .. offset .. " = " .. r_val);
    if (a_val + b_val + offset) ~= r_val then
        error(a_val .. " + " .. b_val .. " + " .. offset .. " does not equal " .. r_val);
    end
end

-- allocate module and input signals. capture outputs
--adder    = Adder_74283.new();
adder    = Adder_CLA_4Bit.new();
ci       = Gate.SIGNAL();
inputs_a = { Gate.SIGNAL(), Gate.SIGNAL(), Gate.SIGNAL(), Gate.SIGNAL() };
inputs_b = { Gate.SIGNAL(), Gate.SIGNAL(), Gate.SIGNAL(), Gate.SIGNAL() };
outputs  = { adder.get_Sum(1), adder.get_Sum(2), adder.get_Sum(3), adder.get_Sum(4), adder.get_CarryOut() };

set_input_vector_value(inputs_a, 0);
set_input_vector_value(inputs_b, 0);

-- set adder inputs and outputs
for i=1,4 do
    adder.set_A(i, inputs_a[i]);
    adder.set_B(i, inputs_b[i]);
end
adder.set_CarryIn(ci);

-- simulate and verify results

ci.set_signal_value(0);

for n=0,15 do
    set_input_vector_value(inputs_a, n);

    for m=0,15 do
        set_input_vector_value(inputs_b, m);

        ncycles = Gate.simulate();
        verify_results(inputs_a, inputs_b, outputs, 0);

    end
end

ci.set_signal_value(1);

-- test with carry in bit set
for n=0,15 do
    set_input_vector_value(inputs_a, n);

    for m=0,15 do
        set_input_vector_value(inputs_b, m);

        ncycles = Gate.simulate();
        verify_results(inputs_a, inputs_b, outputs, 1);
        --io.write('cycles: ' .. ncycles .. '\n');

    end
end

ci.set_signal_value(1);

