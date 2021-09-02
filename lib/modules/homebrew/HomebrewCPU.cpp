#include <lib/modules/homebrew/HomebrewCPU.h>
#include <lib/cpp-api/range.h>

HomebrewCPU_t::HomebrewCPU_t(void) {

    this->register_file.set_data_input(this->data_bus.get_bus());
    this->constants.attach_to_output_bus(this->data_bus.get_bus());
    this->data_bus.connect_to_bus(this->register_file.get_data_output());    



}

void HomebrewCPU_t::TestComponents(void) {

    //RippleCarryAdder_t::Test();
    //RegisterFile_t::Test();
    //AssertConstants_t::Test();
    //OverflowDetector_t::Test();
    //ALU_t::Test();

}
