#include "my_strlen.h"

size_t my_strlen(const char *const string) {
    size_t len = 0;

    __asm (
            "movl $0xffffffff, %%ecx    \n\t"
            "xor %%al, %%al             \n\t"
            "repne scasb                \n\t"

            "not %%ecx                  \n\t"
            "dec %%ecx                  \n\t"
            "movl %%ecx, %0             \n\t"

            : "=D" (len)
            : "D" (string)
            : "%ecx", "%al"
            );

    return len;
}
