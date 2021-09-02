#include <lib/generate/jit/AddressTree.h>

AddressTreeNode_t::AddressTreeNode_t(void)
    :
        bool_arr(false) {

    this->start_addr = NULL;
    this->end_addr = NULL;
    this->parent = NULL;
    this->next_gt = NULL;
    this->next_lt = NULL;
}

AddressTree_t::AddressTree_t(void) {
    this->top = NULL;
    this->cached_node = NULL;
}

AddressTree_t::AddressTree_t(const std::vector<std::pair<logic_gate_t*, logic_gate_t*>>& addrs) {
    this->top = NULL;
    this->cached_node = NULL;
    this->init(addrs);
}

void AddressTree_t::init(const std::vector<std::pair<logic_gate_t*, logic_gate_t*>>& addrs) {
    for(auto& p : addrs)
        this->insert(p);
}

void AddressTree_t::insert(std::pair<logic_gate_t*, logic_gate_t*> addr_info) {

    if(this->top == NULL) {
        this->top = new AddressTreeNode_t();
        this->top->start_addr = addr_info.first;
        this->top->end_addr = addr_info.second;
        return;
    }

    this->insert_(this->top, addr_info);
}

void AddressTree_t::insert_(AddressTreeNode_t* node, const std::pair<logic_gate_t*, logic_gate_t*>& addr_info) {

    if(addr_info.first >= node->start_addr && addr_info.first < node->end_addr) {
        // this address range already exists in this tree
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__));
    }

    const logic_gate_t* ptr = addr_info.first;
    const bool lt = node->next_lt != NULL;
    const bool gt = node->next_gt != NULL;
    const bool is_gt = addr_info.first > node->start_addr;

    if(lt && gt) {
        if(is_gt) {
            return this->insert_(node->next_gt, addr_info);
        }
        else {
            return this->insert_(node->next_lt, addr_info);
        }
    }
    else if(!lt && !gt) {
        AddressTreeNode_t* tn = new AddressTreeNode_t();
        tn->parent     = node;
        tn->start_addr = addr_info.first;
        tn->end_addr   = addr_info.second;

        if(is_gt) {
            node->next_gt = tn;
        }
        else {
            node->next_lt = tn;
        }
    }
    else if(lt && !gt) { // gt free, lt taken



    }
    else if(!lt && gt) { // lt free, gt taken

        

    }
    else {
        // something really bad happened here
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " : unknown internal error");
    }

}

AddressTreeNode_t* AddressTree_t::search(logic_gate_t* addr) {

}

AddressTreeNode_t* AddressTree_t::search_w_exc(logic_gate_t* addr) {

}

