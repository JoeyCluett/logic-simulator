#include <lua5.3/lua.hpp>
#include "../logic-gates.h"
#include "../logic-gate-utility.h"
#include "../logic-gate-allocator.h"
#include "logic_wrapper.h"

extern "C" {

int lua_AND( lua_State* L ) {
    auto* gate_ptr = AND();
    lua_pushlightuserdata(L, static_cast<void*>(gate_ptr));
    return 1;
}

int lua_NAND( lua_State* L ) {
    auto* gate_ptr = NAND();
    lua_pushlightuserdata(L, static_cast<void*>(gate_ptr));
    return 1;
}

int lua_OR( lua_State* L ) {
    auto* gate_ptr = OR();
    lua_pushlightuserdata(L, static_cast<void*>(gate_ptr));
    return 1;
}

int lua_NOR( lua_State* L ) {
    auto* gate_ptr = NOR();
    lua_pushlightuserdata(L, static_cast<void*>(gate_ptr));
    return 1;
}

int lua_XOR( lua_State* L ) {
    auto* gate_ptr = XOR();
    lua_pushlightuserdata(L, static_cast<void*>(gate_ptr));
    return 1;
}

int lua_XNOR( lua_State* L ) {
    auto* gate_ptr = XNOR();
    lua_pushlightuserdata(L, static_cast<void*>(gate_ptr));
    return 1;
}

int lua_FLIPFLOP( lua_State* L ) {
    auto* gate_ptr = FLIPFLOP();
    lua_pushlightuserdata(L, static_cast<void*>(gate_ptr));
    return 1;
}

int lua_CONSTANT( lua_State* L ) {
    int value = luaL_checkinteger(L, 1);
    auto* gate_ptr = CONSTANT(value);
    lua_pushlightuserdata(L, static_cast<void*>(gate_ptr));
    return 1;
}

int lua_SIGNAL( lua_State* L ) {
    auto* gate_ptr = SIGNAL();
    lua_pushlightuserdata(L, static_cast<void*>(gate_ptr));
    return 1;
}

int lua_logic_gate_add_input( lua_State* L ) {
    auto* gate_ptr        = static_cast<logic_gate_t*>(lua_touserdata(L, 1));
    auto* input_gate_ptr  = static_cast<logic_gate_t*>(lua_touserdata(L, 2));
    logic_gate_add_input(gate_ptr, input_gate_ptr);
    return 0;
}

int lua_logic_gate_flipflop_set_clock( lua_State* L ) {
    auto* gate_ptr        = static_cast<logic_gate_t*>(lua_touserdata(L, 1));
    auto* input_gate_ptr  = static_cast<logic_gate_t*>(lua_touserdata(L, 2));
    logic_gate_flipflop_set_clock(gate_ptr, input_gate_ptr);
    return 0;
}

int lua_logic_gate_flipflop_set_data( lua_State* L ) {
    auto* gate_ptr        = static_cast<logic_gate_t*>(lua_touserdata(L, 1));
    auto* input_gate_ptr  = static_cast<logic_gate_t*>(lua_touserdata(L, 2));
    logic_gate_flipflop_set_data(gate_ptr, input_gate_ptr);
    return 0;
}

int lua_logic_gate_simulate( lua_State* L) {
    int sim_cycles = logic_gate_simulate();
    lua_pushinteger(L, sim_cycles);
    return 1;
}

int lua_logic_gate_simulate_callback( lua_State* L) {
    int sim_cycles = logic_gate_simulate();
    lua_pushinteger(L, sim_cycles);
    return 1;
}

} // extern "C"
