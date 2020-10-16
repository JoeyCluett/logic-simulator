package.cpath = package.cpath .. ';./../lib/?.so';
package.path  = package.path  .. ';./../lib/?.lua';
package.path  = package.path  .. ';./../modules/?.lua';
package.path  = package.path  .. ';./../modules/adders/?.lua';

-- necessary imports and setup the backend allocator
require("simload");
require("gate");
allocator_logic_init();

require("Multiplexer");

mux = Multiplexer.new(4); -- 4 select lines, 16 inputs
inputs = {};
selects = {};

-- set inputs
for i=0,15 do
    inputs[i] = Gate.SIGNAL();
    mux.set_Input(i, inputs[i]);
    inputs[i].set_signal_value(0);
end

-- set selects
for i=0,3 do
    selects[i] = Gate.SIGNAL();
    mux.set_Select(i, selects[i]);
    selects[i].set_signal_value(0);
end




