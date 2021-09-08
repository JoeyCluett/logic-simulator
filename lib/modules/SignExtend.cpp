#include <lib/modules/SignExtend.h>

#include <assert.h>

SignExtend_t::SignExtend_t(size_t input_width, size_t output_width) 
    :
        input_forwards(VectorInit_(input_width, FORWARD)),
        output_forwards(VectorInit_(output_width, FORWARD))
{
    assert(input_width <= output_width);


    for(size_t i = 0ul; i < output_width; i++) {
        if(i < input_width) {
            output_forwards.at(i).add_input(input_forwards.at(i));
        }
        else {
            output_forwards.at(i).add_input(input_forwards.back());
        }
    }

}

void SignExtend_t::set_input(std::vector<Gate_t> in) {
    VectorConnect_(this->input_forwards, in);
}

auto SignExtend_t::get_output(void) -> std::vector<Gate_t> {
    return this->output_forwards;
}
