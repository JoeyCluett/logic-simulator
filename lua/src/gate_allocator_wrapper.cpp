#include <lua5.3/lua.hpp>
#include "../logic-gates.h"
#include "../logic-gate-utility.h"
#include "../logic-gate-allocator.h"
#include "logic_wrapper.h"

extern "C" {

int lua_logic_init(lua_State* L) {
    logic_init(); // initialize all logic subsystems
    return 0;
}

// number of gates currently allocated
int lua_logic_allocator_ngates(lua_State* L) {
    long int n = logic_allocator_ngates();
    lua_pushinteger(L, n);
    return 1;
}

// return pointer to pre-allocated logic_gate struct
int lua_logic_gate_alloc(lua_State* L) {
    auto* gate_ptr = logic_gate_alloc();
    lua_pushlightuserdata(L, static_cast<void*>(gate_ptr));
    return 1;
}

// return pointer to pre-allocated logic_input struct
int lua_logic_input_alloc(lua_State* L) {
    auto* input_ptr = logic_input_alloc();
    lua_pushlightuserdata(L, static_cast<void*>(input_ptr));
    return 1;
}

} // extern "C"
