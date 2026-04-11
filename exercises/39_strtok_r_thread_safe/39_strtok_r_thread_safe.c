#include <stdio.h>
#include <stdlib.h>

/*
 * 19 字符串分割器（类似 strtok_r）
 */

static int is_delim(char c, const char *delim) {
    for (const char *d = delim; *d != '\0'; d++) {
        if (c == *d) return 1;
    }
    return 0;
}

char *strtok_r(char *str, const char *delim, char **saveptr) {
    char *start;

    if (str != NULL) {
        start = str;
    } else {
        if (*saveptr == NULL) return NULL;
        start = *saveptr;
    }

    // Skip leading delimiters
    while (*start != '\0' && is_delim(*start, delim)) {
        start++;
    }

    if (*start == '\0') {
        *saveptr = NULL;
        return NULL;
    }

    // Find end of token
    char *end = start;
    while (*end != '\0' && !is_delim(*end, delim)) {
        end++;
    }

    if (*end != '\0') {
        *end = '\0';
        *saveptr = end + 1;
    } else {
        *saveptr = NULL;
    }

    return start;
}

int main(void) {
    char buf[] = "hello,world test";
    const char *delim = ", ";
    char *save = NULL;

    char *tok = strtok_r(buf, delim, &save);
    while (tok) {
        printf("%s\n", tok);
        tok = strtok_r(NULL, delim, &save);
    }
    return 0;
}
