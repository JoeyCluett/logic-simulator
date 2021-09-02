#pragma once

#include <algorithm>
#include <functional>

//
//   for(auto& item : range(begval, endval)) {
//       ...;
//   }
//

template<typename T>
class range_object {
    T mStart, mEnd;
public:
    range_object(T start, T end) : mStart(start), mEnd(end) {}

    template<typename L>
    struct range_object_iterator {
        bool count_up; L val;
        range_object_iterator(L val, bool count_up) : val(val), count_up(count_up) {}
        auto operator++() -> range_object_iterator& { this->count_up ? ++this->val : --this->val; return *this; }
        auto operator++(int) -> range_object_iterator& { range_object_iterator tmp = *this; ++(*this); return tmp; }
        inline bool operator==(const range_object_iterator<L>& rhs) const { return (this->val == rhs.val); }
        inline bool operator!=(const range_object_iterator<L>& rhs) const { return !(*this == rhs); }
        auto operator*() -> L& { return this->val; }
    };

    auto begin(void) const -> range_object_iterator<T> {
        if(this->mStart < this->mEnd) return range_object_iterator<T>(mStart, 1);
        else return range_object_iterator<T>(mStart, 0); }

    auto end(void) const -> range_object_iterator<T> {
        if(this->mStart < this->mEnd) return range_object_iterator<T>(mEnd, 1);
        else return range_object_iterator<T>(mEnd, 0); }
};

template<typename T, typename S>
auto range(T start, S end) -> range_object<T> { return range_object<T>(start, static_cast<T>(end)); }

template<typename T>
class range_object_predicate {
    T mStart, mEnd;
    std::function<bool(T&)> mPred;
public:
    range_object_predicate(T start, T end, std::function<bool(T&)> pred) : mStart(start), mEnd(end), mPred(pred) {}

    template<typename L>
    struct range_object_pred_iter {
        bool count_up; L val; const L* other;
        std::function<bool(L&)> pred;

        range_object_pred_iter(L val, bool count_up, std::function<bool(L&)> pred, const L* other = 0)
                : val(val), count_up(count_up), pred(pred), other(other) {}
        auto operator++() -> range_object_pred_iter& {
            this->count_up ? ++this->val : --this->val;
            while(!this->pred(this->val) && this->val != *(this->other))
                this->count_up ? ++this->val : --this->val;
        }
        auto operator++(int) -> range_object_pred_iter& { range_object_pred_iter tmp = *this; ++(*this); return tmp; }

        inline bool operator==(const range_object_pred_iter<L>& rhs) const { return (this->val == rhs.val); }
        inline bool operator!=(const range_object_pred_iter<L>& rhs) const { return !(*this == rhs); }
        auto operator*() -> L& { return this->val; }
    };

    auto begin(void) const -> range_object_pred_iter<T> {
        if(this->mStart < this->mEnd) return range_object_pred_iter<T>(mStart, 1, mPred, &mEnd);
        else return range_object_pred_iter<T>(mStart, 0, mPred, &mEnd); }

    auto end(void) const -> range_object_pred_iter<T> {
        if(this->mStart < this->mEnd) return range_object_pred_iter<T>(mEnd, 1, mPred, &mStart);
        else return range_object_pred_iter<T>(mEnd, 0, mPred, &mStart); }

};

template<typename T, typename S, typename P>
auto range(T start, S end, P pred) -> range_object_predicate<T> {
    return range_object_predicate<T>(start, static_cast<T>(end), static_cast<std::function<bool(T&)>>(pred)); }



