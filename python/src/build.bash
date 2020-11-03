#!/bin/bash

stdopts="-I../.. -std=c++11 -march=native -O3"
objectfiles="../../logic-gate-allocator.o ../../logic-gate-utility.o ../../logic-gates.o"

# combine python wrapper and .o file to create shared library
options="-I/usr/include/python2.7 -lboost_python -lpython2.7"
g++ -o hdl.so wrapper.cpp $objectfiles -fPIC -shared $stdopts $options
