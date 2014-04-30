#include <stdio.h>

int main() {
#if 1
    __asm__ __volatile__ (
        "pushf\n"
        "orl    $(1 << 8), 0(%rsp)\n"
        "popf\n"
    );
#endif
    puts("Hello, World!");
    return 0;
}
