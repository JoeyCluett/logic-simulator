
require("simload");
require("gate");

Register_NBit = {};

Register_NBit.new = function(bitwidth)

    local self = {};

    for i=0,(bitwidth-1) do
        self.ff[i] = Gate.FLIPFLOP();
    end

    return self;
end