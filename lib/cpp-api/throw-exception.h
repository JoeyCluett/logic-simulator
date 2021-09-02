#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#define THROW_RE(msg) \
    throw std::runtime_error("\n" + std::string(__PRETTY_FUNCTION__) + " : " + msg)

#define THROW_RE_p(premsg, msg) \
    throw std::runtime_error( std::string(premsg) + "\n" + std::string(__PRETTY_FUNCTION__) + " : " msg )

#define TRY_CATCH_RE(expr) \
    try { \
        expr; \
    } \
    catch(std::runtime_error& re_) { \
        std::cerr <<  \
            "\n\n*** RUNTIME ERROR ***\n" << \
            re_.what() << \
                "\n\n*********************\n\n" << \
            std::flush; \
        exit(1); \
    }
