#pragma once

#include <lib/logic.h>
#include <lib/generate/jit/BoolArray.h>

#include <algorithm>
#include <array>
#include <vector>

struct AddressTreeNode_t {
    logic_gate_t* start_addr;
    logic_gate_t* end_addr;

    AddressTreeNode_t* parent;
    AddressTreeNode_t* next_gt;
    AddressTreeNode_t* next_lt;

    // size taken directly from backend logic allocator
    BoolArray_t<LOGIC_GATE_ALLOCATOR_CHUNK_SIZE> bool_arr;

    AddressTreeNode_t(void);

};

struct AddressTree_t {
private:
    AddressTreeNode_t* top;
    AddressTreeNode_t* cached_node;

    //
    // insert algorithm used internally
    //
    void insert_(AddressTreeNode_t* node, const std::pair<logic_gate_t*, logic_gate_t*>& addr_info);

public:
    AddressTree_t(void);
    AddressTree_t(const std::vector<std::pair<logic_gate_t*, logic_gate_t*>>& addrs);

    //
    // initialize tree with given node information
    //
    void init(const std::vector<std::pair<logic_gate_t*, logic_gate_t*>>& addrs);

    //
    // create new node with given information
    //
    void insert(std::pair<logic_gate_t*, logic_gate_t*> addr_info);

    //
    // search for node with given gate information
    //
    AddressTreeNode_t* search(logic_gate_t* addr);

    //
    // search for node with given gate information, throwing exception on error
    //
    AddressTreeNode_t* search_w_exc(logic_gate_t* addr);

};
