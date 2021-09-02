#pragma once

#include <lib/logic.h>

#include <string>
#include <map>
#include <vector>

//
// generate a file with the entire netlist
//
void generate_netlist_file(
        const std::string& filename, 
        const std::string& modulename,
        const std::map<std::string, logic_gate_t*>& inputs,
        const std::map<std::string, std::vector<logic_gate_t*>>& input_vecs,
        const std::map<std::string, logic_gate_t*>& outputs,
        const std::map<std::string, std::vector<logic_gate_t*>>& output_vecs);

void generate_netlist_file_binary(
        const std::string& filename,
        const std::map<std::string, logic_gate_t*>& inputs,
        const std::map<std::string, std::vector<logic_gate_t*>>& input_vecs);

