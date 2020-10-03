
print("defining 'GlobalDebug' package...");

GlobalDebug = {}

-- by default, global debug information is off
GlobalDebug._debug_value = false;

GlobalDebug.set_debug_value = function(boolvalue)
    GlobalDebug._debug_value = boolvalue;
end

GlobalDebug.get_debug_value = function()
    return GlobalDebug._debug_value;
end

--print_wrapper = print;

-- replace print with something more useful
print = function(...)
    if GlobalDebug.get_debug_value() then
        --print_wrapper(unpack(args));    
        -- unpack was deprecated in 5.1 and removed in 5.2

        for _, v in ipairs({...}) do
            io.write(tostring(v));
        end
        io.write("\n");

    end
end


