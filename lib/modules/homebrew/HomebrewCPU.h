#pragma once

#include <lib/logic.h>
#include <lib/cpp-api/gate-wrapper.h>

// general purpose modules
#include <lib/modules/adders/RippleCarryAdder.h>

// modules for homebrew cpu
#include <lib/modules/homebrew/RegisterFile.h>
#include <lib/modules/homebrew/AssertConstants.h>
#include <lib/modules/homebrew/CpuBus.h>
#include <lib/modules/homebrew/OverflowDetector.h>
#include <lib/modules/homebrew/ALU.h>

struct HomebrewCPU_t {

    ALU_t             alu;
    RegisterFile_t    register_file;
    AssertConstants_t constants;

    CpuBus_t          data_bus;
    CpuBus_t          address_bus;

    //
    // default ctor connects all of the internal modules
    //
    HomebrewCPU_t(void);

    //
    // calls Test() method for all internal components
    //
    static void TestComponents(void);

};
