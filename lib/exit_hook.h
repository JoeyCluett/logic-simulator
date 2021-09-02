#pragma once

#include <stdio.h>
#include <stdarg.h>

//#define TRACE_ON_EXIT

#ifdef __cplusplus
extern "C" {
#endif

void exit_hook(FILE* fptr, const char* format, ...);

#ifdef __cplusplus
} // __cplusplus
#endif
