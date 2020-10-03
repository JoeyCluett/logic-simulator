#!/bin/bash

# build logic library
# also preps files for use in Lua interface library
gcc -c -o logic-gates.o          logic-gates.c          -O3 -march=native -I. -fPIC
gcc -c -o logic-gate-utility.o   logic-gate-utility.c   -O3 -march=native -I. -fPIC
gcc -c -o logic-gate-allocator.o logic-gate-allocator.c -O3 -march=native -I. -fPIC
gcc -c -o logic-vector.o         logic-vector.c         -O3 -march=native -I. -fPIC

# build modules
gcc -o main main.c logic-gates.o logic-gate-utility.o logic-gate-allocator.o logic-vector.o -O3 -march=native -I.
