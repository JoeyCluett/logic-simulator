#include <lib/modules/BitwiseEqualDetect.h>
#include <lib/cpp-api/gate-wrapper.h>

#include <assert.h>

BitwiseEqualDetect_t::BitwiseEqualDetect_t(size_t width)
    :
        andgate(AND()),
        xorgates(VectorInit_(width, XNOR)),
        forward_a(VectorInit_(width, FORWARD)),
        forward_b(VectorInit_(width, FORWARD)) {

    for(size_t i = 0UL; i < width; i++) {
        this->xorgates.at(i).add_input(this->forward_a.at(i));
        this->xorgates.at(i).add_input(this->forward_b.at(i));
        this->andgate.add_input(this->xorgates.at(i));
    }
}

Gate_t BitwiseEqualDetect_t::get_output(void) {
    return this->andgate;
}

void BitwiseEqualDetect_t::set_a_inputs(std::vector<Gate_t> a) {
    assert(a.size() == this->forward_a.size());
    VectorConnect_(this->forward_a, a);
}

void BitwiseEqualDetect_t::set_b_inputs(std::vector<Gate_t> b) {
    assert(b.size() == this->forward_b.size());
    VectorConnect_(this->forward_b, b);
}

BitwiseEqualDetect_t::operator Gate_t() {
    return this->andgate;
}
