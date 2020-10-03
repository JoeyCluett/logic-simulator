
require("simload");
require("gate");

BitVector = {}

BitVector.new = function(sz)

    local self = {}

    for i=1,sz do
        -- table will be zero-indexed
        self[i-1] = Gate.SIGNAL();
    end

    self.subvector = function(start_idx, end_idx)
        local tab = {};

        for i=start_idx,(end_idx-1) do
            -- returned array is zero-indexed
            tab[i-start_idx] = self[i];
        end

        return tab;
    end

    self.set_values = function(bitstring)

        local idx = 0;

        for v in string.gmatch(bitstring, ".") do
            if v == '0' then
                self[idx].set_signal_value(0);
            elseif v == '1' then
                self[idx].set_signal_value(1);
            else
                error('BitVector.set_values : invalid signal value');
            end

            idx = idx + 1;
        end

    end

    return self;
end

