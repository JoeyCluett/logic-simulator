#include <lib/exit_hook.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef TRACE_ON_EXIT
#include <execinfo.h>
#include <cxxabi.h>
#include <signal.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

void exit_hook(FILE* fptr, const char* format, ...) {

    fprintf(fptr, "ERR > ");



    // forward variadic args to vfprintf
    va_list ap;
    va_start(ap, format);
    vfprintf(fptr, format, ap);
    va_end(ap);

#ifdef TRACE_ON_EXIT
    raise(SIGINT);
#else
    exit(1);
#endif
}


#ifdef __cplusplus
} // __cplusplus
#endif
