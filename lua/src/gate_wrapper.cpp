#include <lua5.3/lua.hpp>
#include "../logic-gates.h"
#include "../logic-gate-utility.h"
#include "../logic-gate-allocator.h"
#include "logic_wrapper.h"

extern "C" {

// heap allocate gate and initialize
int lua_logic_gate_new( lua_State* L ) {
    int  gate_type = (int)luaL_checkinteger(L, 1);
    auto* gate_ptr = logic_gate_new(static_cast<logic_type>(gate_type));
    
    lua_pushlightuserdata(L, static_cast<void*>(gate_ptr));
    return 1;
}

// initialize a pre-allocated gate with specific type
int lua_logic_gate_init( lua_State* L ) {
    auto* gate_ptr  = static_cast<logic_gate_t*>(lua_touserdata(L, 1));
    int   gate_type = (int)luaL_checkinteger(L, 2);
    
    gate_ptr = logic_gate_init(gate_ptr, static_cast<logic_type>(gate_type));
    return 0;
}

// set the output of gate
int lua_logic_gate_signal_set( lua_State* L ) {
    auto* gate_ptr = static_cast<logic_gate_t*>(lua_touserdata(L, 1));
    int   gate_val = (int)luaL_checkinteger(L, 2);

    logic_gate_signal_set(gate_ptr, gate_val);
    return 0;
}

} // extern "C"

