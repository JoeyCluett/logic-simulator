#pragma once

#include <functional>
#include <lib/logic-gate-allocator.h>

const int CallbackWrap_PreCall = 0;
const int CallbackWrap_PostCall = 1;

struct CallbackWrapper {

    static CallbackWrapper* precallback_obj_ptr;
    static CallbackWrapper* postcallback_obj_ptr;

    static void registered_pre_callback(void);
    static void registered_post_callback(void);

    const int callback_preference;
    std::function<void(void)> cb;

    CallbackWrapper(std::function<void(void)> callable, const int callback_preference);

    ~CallbackWrapper();
};

// initialize static members
CallbackWrapper* CallbackWrapper::precallback_obj_ptr = NULL;
CallbackWrapper* CallbackWrapper::postcallback_obj_ptr = NULL;

CallbackWrapper::CallbackWrapper(std::function<void(void)> callable, const int callback_preference)
    :
        callback_preference(callback_preference),
        cb(callable) {

    logic_gate_simulate_set_precallback(CallbackWrapper::registered_pre_callback);
    logic_gate_simulate_set_postcallback(CallbackWrapper::registered_post_callback);

    if(callback_preference == CallbackWrap_PreCall) {
        CallbackWrapper::precallback_obj_ptr = this;
    }
    else if(callback_preference == CallbackWrap_PostCall) {
        CallbackWrapper::postcallback_obj_ptr = this;
    }
    else {
        throw 1;
    }
}

CallbackWrapper::~CallbackWrapper() {
    if(callback_preference == CallbackWrap_PreCall) {
        CallbackWrapper::precallback_obj_ptr = NULL;
        logic_gate_simulate_set_precallback(NULL);
    }
    else if(callback_preference == CallbackWrap_PostCall) {
        CallbackWrapper::postcallback_obj_ptr = NULL;
        logic_gate_simulate_set_postcallback(NULL);
    }
}

void CallbackWrapper::registered_pre_callback(void) {
    CallbackWrapper::precallback_obj_ptr->cb();
}

void CallbackWrapper::registered_post_callback(void) {
    CallbackWrapper::postcallback_obj_ptr->cb();
}


