#include <lua5.3/lua.hpp>
#include "../logic-gates.h"
#include "../logic-gate-utility.h"
#include "../logic-gate-allocator.h"

extern "C" {

// ====================================================
// allocator functions
// ====================================================

// initialize logic subsystem
int lua_logic_init(lua_State* L);

// number of gates currently allocated
int lua_logic_allocator_ngates(lua_State* L);

// return pointer to pre-allocated logic_gate struct
int lua_logic_gate_alloc(lua_State* L);

// return pointer to pre-allocated logic_input struct
int lua_logic_input_alloc(lua_State* L);

// ====================================================
// utility functions
// ====================================================

// wrappers over gate allocation/initialization functions
int lua_AND( lua_State* L );
int lua_NAND( lua_State* L );
int lua_OR( lua_State* L );
int lua_NOR( lua_State* L );
int lua_XOR( lua_State* L );
int lua_XNOR( lua_State* L );
int lua_FLIPFLOP( lua_State* L );
int lua_CONSTANT( lua_State* L );
int lua_SIGNAL( lua_State* L );

// adds gate input. input gate is pointer to another logic gate
int lua_logic_gate_add_input( lua_State* L );

// wrappers over logic_gate_t::flipflop functionality
int lua_logic_gate_flipflop_set_clock( lua_State* L );
int lua_logic_gate_flipflop_set_data( lua_State* L );

// actual meat of the simulation occurs with this function. this function already 
// knows which gates are relevant to the simulation
int lua_logic_gate_simulate( lua_State* L);

// modified simulate function. calls the callback every cycle
// in this case, the callback will be a Lua function
int lua_logic_gate_simulate_callback( lua_State* L);

// ====================================================
// gate functions
// ====================================================

// heap allocate gate and initialize
int lua_logic_gate_new( lua_State* L );

// initialize a pre-allocated gate with specific type
int lua_logic_gate_init( lua_State* L );

// set the output of gate
int lua_logic_gate_signal_set( lua_State* L );

// ====================================================
// expose to Lua
// ====================================================

int luaopen_logic_wrapper(lua_State* L);

} // extern "C"
