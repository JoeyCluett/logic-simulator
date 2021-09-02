#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

template<typename GType>
void V_add_input_(GType g, GType g_) {
    g.add_input(g_);
}

template<typename GType>
void V_cycle_(GType base, GType first) {
    V_add_input_(base, first);
}

template<typename GType, typename... Others>
void V_cycle_(GType base, GType first, Others... others) {
    V_cycle_(base, first);
    V_cycle_(base, others...);
}

template<typename Callable, typename GType>
Gate_t V_FirstCall_(Callable init_callback, GType first) {
    Gate_t basegate(init_callback());
    V_cycle_(basegate, first);
    return basegate;
}

template<typename Callable, typename GType, typename... Others>
Gate_t V_FirstCall_(Callable init_callback, GType first, Others... others) {
    Gate_t basegate(init_callback());
    V_cycle_(basegate, first, others...);
    return basegate;
}

// ===========================================================
// and gate varargs type
// ===========================================================

template<typename GType>
Gate_t And_(GType first) {
    return V_FirstCall_(AND, first);
}

template<typename GType, typename... Others>
Gate_t And_(GType first, Others... others) {
    return V_FirstCall_(AND, first, others...);
}

// ===========================================================
// nand gate varargs type
// ===========================================================

template<typename GType>
Gate_t Nand_(GType first) {
    return V_FirstCall_(NAND, first);
}

template<typename GType, typename... Others>
Gate_t Nand_(GType first, Others... others) {
    return V_FirstCall_(NAND, first, others...);
}

// ===========================================================
// or gate varargs type
// ===========================================================

template<typename GType>
Gate_t Or_(GType first) {
    return V_FirstCall_(OR, first);
}

template<typename GType, typename... Others>
Gate_t Or_(GType first, Others... others) { 
    return V_FirstCall_(OR, first, others...);
}

// ===========================================================
// nor gate varargs type
// ===========================================================

template<typename GType>
Gate_t Nor_(GType first) {
    return V_FirstCall_(NOR, first);
}

template<typename GType, typename... Others>
Gate_t Nor_(GType first, Others... others) {
    return V_FirstCall_(NOR, first, others...);
}

// ===========================================================
// xor gate varargs type
// ===========================================================

template<typename GType>
Gate_t Xor_(GType first) {
    return V_FirstCall_(XOR, first);
}

template<typename GType, typename... Others>
Gate_t Xor_(GType first, Others... others) {
    return V_FirstCall_(XOR, first, others...);
}

// ===========================================================
// xnor gate varargs type
// ===========================================================

template<typename GType>
Gate_t Xnor_(GType first) {
    return V_FirstCall_(XNOR, first);
}

template<typename GType, typename... Others>
Gate_t Xnor_(GType first, Others... others) {
    return V_FirstCall_(XNOR, first, others...);
}

