#include <stdio.h>
#include "../inc/my_strlen.h"

extern char *my_strncpy(char *dst, const char *src, size_t n);

int main(void) {
    char old_str[15] = "Hello, world!";
    char new_str[15] = "\0";

    size_t len_str = my_strlen(old_str);
    printf("len_str = %zu\n", len_str);

    my_strncpy(new_str, old_str, len_str);
    printf("new str = \"%s\"\n", new_str);

    return 0;
}
