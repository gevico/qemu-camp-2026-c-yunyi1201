#include <stdio.h>

unsigned int gcd_asm(unsigned int a, unsigned int b) {
    unsigned int result;

#if defined(__x86_64__) || defined(__i386__)
    __asm__ volatile (
        "mov %1, %%eax\n\t"     // 将a存入eax
        "mov %2, %%ebx\n\t"     // 将b存入ebx
        "jmp .L_check\n\t"      // 跳转到检查条件

        ".L_loop:\n\t"
        "   xor %%edx, %%edx\n\t"
        "   div %%ebx\n\t"       // eax / ebx, 余数在edx
        "   mov %%ebx, %%eax\n\t"
        "   mov %%edx, %%ebx\n\t"

        ".L_check:\n\t"
        "   test %%ebx, %%ebx\n\t"
        "   jne .L_loop\n\t"

        "mov %%eax, %0"
        : "=r" (result)
        : "r" (a), "r" (b)
        : "eax", "ebx", "edx", "cc"
    );
#else
    // Fallback for non-x86: pure C Euclidean algorithm
    unsigned int x = a, y = b;
    while (y != 0) {
        unsigned int tmp = y;
        y = x % y;
        x = tmp;
    }
    result = x;
#endif

    return result;
}

int main(int argc, char* argv[]) {
    printf("%d\n", gcd_asm(12, 8));
    printf("%d\n", gcd_asm(7, 5));
    return 0;
}
