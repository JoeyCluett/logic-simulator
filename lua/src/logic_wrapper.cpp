#include <stdio.h>
#include <lua5.3/lua.hpp>
#include "../logic-gates.h"
#include "../logic-gate-utility.h"
#include "../logic-gate-allocator.h"
#include "logic_wrapper.h"

extern "C" {

// this function does not have any uses outside of the Lua-C interface
static int lua_get_gate_output(lua_State* L) {
    auto* gate_ptr  = static_cast<logic_gate_t*>(lua_touserdata(L, 1));
    lua_pushinteger(L, gate_ptr->output_value);
    return 1;
}

static const struct luaL_Reg regarray[] = {

    // allocator functions

    { "allocator_logic_init",             lua_logic_init },
    { "allocator_logic_allocator_ngates", lua_logic_allocator_ngates },
    { "allocator_logic_gate_alloc",       lua_logic_gate_alloc },
    { "allocator_logic_input_alloc",      lua_logic_input_alloc },

    // utility functions

    { "utility_AND",  lua_AND },
    { "utility_NAND", lua_NAND },
    { "utility_OR",   lua_OR },
    { "utility_NOR",  lua_NOR },
    { "utility_XOR",  lua_XOR },
    { "utility_XNOR", lua_XNOR },
    { "utility_FLIPFLOP", lua_FLIPFLOP },
    { "utility_CONSTANT", lua_CONSTANT },
    { "utility_SIGNAL",   lua_SIGNAL },
    { "utility_logic_gate_add_input",          lua_logic_gate_add_input },
    { "utility_logic_gate_flipflop_set_clock", lua_logic_gate_flipflop_set_clock },
    { "utility_logic_gate_flipflop_set_data",  lua_logic_gate_flipflop_set_data },
    { "utility_logic_gate_simulate",           lua_logic_gate_simulate },
    { "utility_logic_gate_simulate_callback",  lua_logic_gate_simulate_callback },

    // gate functions

    { "logic_gate_new",  lua_logic_gate_new },
    { "logic_gate_init", lua_logic_gate_init },
    { "logic_gate_signal_set", lua_logic_gate_signal_set },
    { "logic_gate_get_output", lua_get_gate_output },

    // array ends with NULL, NULL

    { NULL, NULL }
};

int luaopen_logic_wrapper(lua_State* L) {
    printf("luaopen_logic_wrapper : placing functions in current namespace...\n");
    luaL_newlib(L, regarray);
    return 1;
}

} // extern "C"
