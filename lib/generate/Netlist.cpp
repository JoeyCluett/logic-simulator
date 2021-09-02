#include <lib/generate/Netlist.h>

#include <lib/logic.h>
#include <lib/logic-gate-allocator.h>
#include <lib/exit_hook.h>

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <string>
#include <map>
#include <vector>

static std::map<logic_gate_t*, std::string> generate_full_input_map(
        const std::map<std::string, logic_gate_t*>& inputs,
        const std::map<std::string, std::vector<logic_gate_t*>>& input_vecs) {

    std::map<logic_gate_t*, std::string> ptr_to_string;

    for(auto& p : inputs) {

        std::string name = p.first;
        logic_gate_t* gate_ptr = p.second;
        
        auto iter = ptr_to_string.find(gate_ptr);
        if(iter != ptr_to_string.end())
            throw std::runtime_error("generate_full_input_map : entry for given gate already exists");

        ptr_to_string.insert({ gate_ptr, name });
    }

    for(auto& p : input_vecs) {

        std::string name = p.first;
        std::vector<logic_gate_t*> vec = p.second;

        size_t idx = 0UL;
        for(auto ptr : vec) {

            auto iter = ptr_to_string.find(ptr);
            if(iter != ptr_to_string.end())
                throw std::runtime_error("generate_full_input_map : entry for given gate already exists");

            ptr_to_string.insert({ ptr, name + "[" + std::to_string(idx) + "]" });
            idx++;
        }

    }

    return ptr_to_string;
}

//
// follow forward chains if needed
//
static inline logic_gate_t* fully_evaluate_gate_ptr(logic_gate_t* gate_ptr) {
    logic_gate_t* tmp_ptr = logic_gate_follow(gate_ptr);
    assert(tmp_ptr != NULL);
    return tmp_ptr;
}

static const char* gate_address_to_string(const logic_gate_t* gateptr) {

    static_assert(sizeof(logic_gate_t*) == sizeof(unsigned long));

    static char buf[17];
    memset(buf, '\0', 17);

    union {
        logic_gate_t* _gateptr;
        unsigned long u64;
    };

    _gateptr = (logic_gate_t*)gateptr;

    int buf_idx = 0;
    while(u64) {
        int nibble_value = u64 & 0x0F;
        buf[buf_idx] = "0123456789ABCDEF"[nibble_value];
        u64 >>= 4;
        buf_idx++;
    }

    return buf;
}

static inline const char get_type_prefix(const logic_type type) {
    switch(type) {
        case logic_gate_and:  return 'g';
        case logic_gate_nand: return 'g';
        case logic_gate_or:   return 'g';
        case logic_gate_nor:  return 'g';
        case logic_gate_xor:  return 'g';
        case logic_gate_xnor: return 'g';
        case logic_gate_flipflop: return 'f';
        case logic_gate_buffer:   return 'b';
        case logic_gate_wire:     return 'w';
        default:
            return 'Z';
            //exit_hook(stderr, "in Netlist.cpp, get_type_prefix : invalid gate type '%d' does not have a prefix\n", (int)ptr->gate_type);
    }
}

static inline void print_full_gate_name(FILE* fptr, logic_gate_t* gptr, std::map<logic_gate_t*, std::string>& input_map, const char* subtype_prefix) {
    if(gptr == ZERO) {
        fprintf(fptr, " %s0", subtype_prefix);
    }
    else if(gptr == ONE) {
        fprintf(fptr, " %s1", subtype_prefix);
    }
    else {
        auto iter = input_map.find(gptr);
        if(iter == input_map.end()) {
            // internal gate
            fprintf(fptr, " %s%c%s", subtype_prefix, get_type_prefix(gptr->gate_type), gate_address_to_string(gptr));
        }
        else {
            // input signal
            fprintf(fptr, " %s%s", subtype_prefix, iter->second.c_str());
        }
    }
}

void generate_netlist_file(
        const std::string& filename, 
        const std::string& modulename,
        const std::map<std::string, logic_gate_t*>& inputs,
        const std::map<std::string, std::vector<logic_gate_t*>>& input_vecs,
        const std::map<std::string, logic_gate_t*>& outputs,
        const std::map<std::string, std::vector<logic_gate_t*>>& output_vecs) {

    FILE* fptr = fopen(filename.c_str(), "w");

    fprintf(fptr, 
        "# ====================================\n"
        "# this file is auto-generated\n"
        "# ====================================\n\n");

    fprintf(fptr, ":module %s;\n", modulename.c_str());

    for(auto& p : inputs) {
        std::string name       = p.first;
        logic_gate_t* gate_ptr = p.second;
        fprintf(fptr, ":i %s;\n", name.c_str());
    }

    for(auto& p : input_vecs) {
        std::string name = p.first;
        const std::vector<logic_gate_t*>& vec = p.second;
        fprintf(fptr, ":iv %s %lu;\n", name.c_str(), (unsigned long)vec.size());
    }

    auto write_vec_gate_name = [](FILE* fptr, const char* type_specifier, char type_prefix, std::vector<logic_gate_t*>& vec) -> void {
        fprintf(fptr, "%s", type_specifier);
        for(auto* ptr : vec)
            fprintf(fptr, " %c%s", type_prefix, gate_address_to_string(ptr));
        fprintf(fptr, ";\n");
    };

    {
        std::vector<logic_gate_t*> and_vec;
        std::vector<logic_gate_t*> nand_vec;
        std::vector<logic_gate_t*> or_vec;
        std::vector<logic_gate_t*> nor_vec;
        std::vector<logic_gate_t*> xor_vec;
        std::vector<logic_gate_t*> xnor_vec;
        std::vector<logic_gate_t*> ff_vec;
        std::vector<logic_gate_t*> buf_vec;
        std::vector<logic_gate_t*> wire_vec;

                const size_t max_vec_size = 12UL;

        const logic_allocator_t* alloc = logic_allocator_get_reference();
        logic_gate_chunk_t* chunk = alloc->gate_first;

        while(chunk != NULL) {

            logic_gate_t* gateptr = chunk->gate_chunk;
            const size_t n_allocated = chunk->n_allocated;

            for(size_t idx = 0UL; idx < n_allocated; idx++) {
                switch(gateptr[idx].gate_type) {
                    case logic_gate_and:  and_vec.push_back(gateptr + idx); break;
                    case logic_gate_nand: nand_vec.push_back(gateptr + idx); break;
                    case logic_gate_or:   or_vec.push_back(gateptr + idx); break;
                    case logic_gate_nor:  nor_vec.push_back(gateptr + idx); break;
                    case logic_gate_xor:  xor_vec.push_back(gateptr + idx); break;
                    case logic_gate_xnor: xnor_vec.push_back(gateptr + idx); break;
                    case logic_gate_flipflop: ff_vec.push_back(gateptr + idx); break;
                    case logic_gate_buffer:   buf_vec.push_back(gateptr + idx); break;
                    case logic_gate_wire:     wire_vec.push_back(gateptr + idx); break;
                    default:
                        break;
                }


                if(and_vec.size() == max_vec_size)  { write_vec_gate_name(fptr, ":and ", get_type_prefix(logic_gate_and),      and_vec);  and_vec.clear(); }
                if(nand_vec.size() == max_vec_size) { write_vec_gate_name(fptr, ":nand", get_type_prefix(logic_gate_nand),     nand_vec); nand_vec.clear(); }
                if(or_vec.size() == max_vec_size)   { write_vec_gate_name(fptr, ":or  ", get_type_prefix(logic_gate_or),       or_vec);   or_vec.clear(); }
                if(nor_vec.size() == max_vec_size)  { write_vec_gate_name(fptr, ":nor ", get_type_prefix(logic_gate_nor),      nor_vec);  nor_vec.clear(); }
                if(xor_vec.size() == max_vec_size)  { write_vec_gate_name(fptr, ":xor ", get_type_prefix(logic_gate_xor),      xor_vec);  xor_vec.clear(); }
                if(xnor_vec.size() == max_vec_size) { write_vec_gate_name(fptr, ":xnor", get_type_prefix(logic_gate_xnor),     xnor_vec); xnor_vec.clear(); }
                if(ff_vec.size() == max_vec_size)   { write_vec_gate_name(fptr, ":ff  ", get_type_prefix(logic_gate_flipflop), ff_vec);   ff_vec.clear(); }
                if(buf_vec.size() == max_vec_size)  { write_vec_gate_name(fptr, ":buf ", get_type_prefix(logic_gate_buffer),   buf_vec);  buf_vec.clear(); }
                if(wire_vec.size() == max_vec_size) { write_vec_gate_name(fptr, ":w   ", get_type_prefix(logic_gate_wire),     wire_vec); wire_vec.clear(); }

            }

            chunk = chunk->next;
        }


        if(and_vec.size()  > 0UL) { write_vec_gate_name(fptr, ":and ", get_type_prefix(logic_gate_and),      and_vec);  and_vec.clear(); }
        if(nand_vec.size() > 0UL) { write_vec_gate_name(fptr, ":nand", get_type_prefix(logic_gate_nand),     nand_vec); nand_vec.clear(); }
        if(or_vec.size()   > 0UL) { write_vec_gate_name(fptr, ":or  ", get_type_prefix(logic_gate_or),       or_vec);   or_vec.clear(); }
        if(nor_vec.size()  > 0UL) { write_vec_gate_name(fptr, ":nor ", get_type_prefix(logic_gate_nor),      nor_vec);  nor_vec.clear(); }
        if(xor_vec.size()  > 0UL) { write_vec_gate_name(fptr, ":xor ", get_type_prefix(logic_gate_xor),      xor_vec);  xor_vec.clear(); }
        if(xnor_vec.size() > 0UL) { write_vec_gate_name(fptr, ":xnor", get_type_prefix(logic_gate_xnor),     xnor_vec); xnor_vec.clear(); }
        if(ff_vec.size()   > 0UL) { write_vec_gate_name(fptr, ":ff  ", get_type_prefix(logic_gate_flipflop), ff_vec);   ff_vec.clear(); }
        if(buf_vec.size()  > 0UL) { write_vec_gate_name(fptr, ":buf ", get_type_prefix(logic_gate_buffer),   buf_vec);  buf_vec.clear(); }
        if(wire_vec.size() > 0UL) { write_vec_gate_name(fptr, ":w   ", get_type_prefix(logic_gate_wire),     wire_vec); wire_vec.clear(); }


    }

    for(auto& p : outputs) {
        std::string name       = p.first;
        logic_gate_t* gate_ptr = p.second;
        fprintf(fptr, ":o %s;\n", name.c_str());
    }

    for(auto& p : output_vecs) {
        std::string name = p.first;
        const std::vector<logic_gate_t*>& vec = p.second;
        fprintf(fptr, ":ov %s %lu;\n", name.c_str(), (unsigned long)vec.size());
    }

    fprintf(fptr, 
        "\n# =============================================\n"
        "# generating internal connections\n"
        "# =============================================\n\n");

    auto input_map = generate_full_input_map(inputs, input_vecs);

    {
        const logic_allocator_t* alloc = logic_allocator_get_reference();
        logic_gate_chunk_t* chunk = alloc->gate_first;

        while(chunk != NULL) {

            const size_t n_allocated       = chunk->n_allocated;
            logic_gate_t* gateptr          = chunk->gate_chunk;
            const logic_gate_t* gateendptr = chunk->gate_chunk + n_allocated;
            
            while(gateptr != gateendptr) {
                switch(gateptr->gate_type) {
                    case logic_gate_and:
                    case logic_gate_nand:
                    case logic_gate_or:
                    case logic_gate_nor:
                    case logic_gate_xor:
                    case logic_gate_xnor:
                        {
                            fprintf(fptr, "%c%s", get_type_prefix(gateptr->gate_type), gate_address_to_string(gateptr));
                            // iterate through all inputs
                            logic_input_t* input_ptr = gateptr->gate.inputs;
                            while(input_ptr != NULL) {
                                logic_gate_t* ptr = fully_evaluate_gate_ptr(input_ptr->output_ptr);
                                print_full_gate_name(fptr, ptr, input_map, "");
                                input_ptr = input_ptr->next;
                            }
                            fputs(";\n", fptr);
                        }
                        break;

                    case logic_gate_flipflop:
                        {
                            fprintf(fptr, "%c%s", get_type_prefix(logic_gate_flipflop), gate_address_to_string(gateptr));

                            logic_gate_t* clk  = fully_evaluate_gate_ptr(gateptr->flipflop.clk_input);
                            logic_gate_t* data = fully_evaluate_gate_ptr(gateptr->flipflop.data_input);

                            print_full_gate_name(fptr, clk, input_map, "c:");
                            print_full_gate_name(fptr, data, input_map, "d:");

                            fputs(";\n", fptr);
                        }
                        break;

                    case logic_gate_buffer:
                        {
                            fprintf(fptr, "%c%s", get_type_prefix(logic_gate_buffer), gate_address_to_string(gateptr));

                            logic_gate_t* data = fully_evaluate_gate_ptr(gateptr->tristate_buffer.data_input);
                            logic_gate_t* sel  = fully_evaluate_gate_ptr(gateptr->tristate_buffer.select_input);

                            print_full_gate_name(fptr, data, input_map, "d:");
                            print_full_gate_name(fptr, sel, input_map, "s:");

                            fputs(";\n", fptr);
                        }
                        break;

                    case logic_gate_wire:
                        {
                            fprintf(fptr, "%c%s", get_type_prefix(logic_gate_wire), gate_address_to_string(gateptr));

                            logic_input_t* input_ptr = gateptr->wire.input_list;
                            while(input_ptr != NULL) {
                                logic_gate_t* ptr = fully_evaluate_gate_ptr(input_ptr->output_ptr);
                                print_full_gate_name(fptr, ptr, input_map, "");
                                input_ptr = input_ptr->next;
                            }

                            fputs(";\n", fptr);
                        }
                        break;

                    default:
                        break;
                }

                gateptr++;
            }

            chunk = chunk->next;
        }

    }

    fclose(fptr);
}

void generate_netlist_file_binary(
        const std::string& filename,
        const std::map<std::string, logic_gate_t*>& inputs,
        const std::map<std::string, std::vector<logic_gate_t*>>& input_vecs) {

    const unsigned char magic[8] = { 0xDE, 0xC0, 0xDE, 0x1, 0x0, 0x9, 0x1, 0xC }; // DECODElogic    

    FILE* fptr = fopen(filename.c_str(), "wb");
    fwrite(magic, 1, 8, fptr);

    fclose(fptr);
}








