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
allocator_logic_init();

-- import module and connect inputs
require("full_adder");

fa = FullAdder.new()
bv = BitVector.new(3);

fa.set_A(bv[0]);
fa.set_B(bv[1]);
fa.set_CarryIn(bv[2]);

sum = fa.get_Sum();
car = fa.get_CarryOut();

print('simulating...')

bv.set_values('000');
Gate.simulate();
print("000 = sum: ", sum.get_value(), ", carry: ", car.get_value());

bv.set_values('001');
Gate.simulate();
print("001 = sum: ", sum.get_value(), ", carry: ", car.get_value());

bv.set_values('010');
Gate.simulate();
print("010 = sum: ", sum.get_value(), ", carry: ", car.get_value());

bv.set_values('011');
Gate.simulate();
print("011 = sum: ", sum.get_value(), ", carry: ", car.get_value());

bv.set_values('100');
Gate.simulate();
print("100 = sum: ", sum.get_value(), ", carry: ", car.get_value());

bv.set_values('101');
Gate.simulate();
print("101 = sum: ", sum.get_value(), ", carry: ", car.get_value());

bv.set_values('110');
Gate.simulate();
print("110 = sum: ", sum.get_value(), ", carry: ", car.get_value());

bv.set_values('111');
Gate.simulate();
print("111 = sum: ", sum.get_value(), ", carry: ", car.get_value());

