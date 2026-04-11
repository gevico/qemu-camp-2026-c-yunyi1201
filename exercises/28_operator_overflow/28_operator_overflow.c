#include <stdio.h>
#include <limits.h>

#define CHECK_OVERFLOW(carry) \
    carry ? "Overflow" : "Not Overflow"

int check_add_overflow_asm(unsigned int a, unsigned int b) {
#if defined(__x86_64__) || defined(__i386__)
    unsigned char carry;
    __asm__ volatile(
        "addl %2, %1\n\t"
        "setc %0\n\t"
        : "=r" (carry), "+r" (a)
        : "r" (b)
        : "cc"
    );
    return carry;
#else
    return (a > UINT_MAX - b) ? 1 : 0;
#endif
}

int check_sub_overflow_asm(unsigned int a, unsigned int b) {
#if defined(__x86_64__) || defined(__i386__)
    unsigned char carry;
    __asm__ volatile(
        "subl %2, %1\n\t"
        "setc %0\n\t"
        : "=r" (carry), "+r" (a)
        : "r" (b)
        : "cc"
    );
    return carry;
#else
    return (b > a) ? 1 : 0;
#endif
}

int check_mul_overflow_asm(unsigned int a, unsigned int b) {
#if defined(__x86_64__) || defined(__i386__)
    unsigned int high_bits;
    unsigned char overflow;
    __asm__ volatile(
        "mull %3\n\t"
        "setc %0\n\t"
        "mov %%edx, %1\n\t"
        : "=r" (overflow), "=r" (high_bits), "+a" (a)
        : "r" (b)
        : "edx", "cc"
    );
    return overflow || (high_bits != 0);
#else
    if (a == 0 || b == 0) return 0;
    return (a > UINT_MAX / b) ? 1 : 0;
#endif
}

int check_div_overflow_asm(unsigned int a, unsigned int b) {
#if defined(__x86_64__) || defined(__i386__)
    unsigned char is_div_zero;
    __asm__ volatile(
        "testl %1, %1\n\t"
        "sete %0\n\t"
        : "=r" (is_div_zero)
        : "r" (b)
        : "cc"
    );
    return is_div_zero;
#else
    return (b == 0) ? 1 : 0;
#endif
}

int main() {
    printf("(UINT_MAX + 1)Add: %s\n", CHECK_OVERFLOW(check_add_overflow_asm(UINT_MAX, 1)));
    printf("(1, 0)Add: %s\n", CHECK_OVERFLOW(check_add_overflow_asm(1, 0)));
    printf("(0, 1)Sub: %s\n", CHECK_OVERFLOW(check_sub_overflow_asm(0, 1)));
    printf("(2, 1)Sub: %s\n", CHECK_OVERFLOW(check_sub_overflow_asm(2, 1)));
    printf("(UINT_MAX, 2)Mul: %s\n", CHECK_OVERFLOW(check_mul_overflow_asm(UINT_MAX, 2)));
    printf("(1, 2)Mul: %s\n", CHECK_OVERFLOW(check_mul_overflow_asm(1, 2)));
    printf("(10, 0)Div: %s\n", CHECK_OVERFLOW(check_div_overflow_asm(10, 0)));
    printf("(2, 1)Div: %s\n", CHECK_OVERFLOW(check_div_overflow_asm(2, 1)));
    return 0;
}
