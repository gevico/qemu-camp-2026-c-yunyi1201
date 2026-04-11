#include <stdio.h>

int main() {
    char str[]="Don't ask what your country can do for you, but ask what you can do for your country.";
    int wordCount = 0;

    int inWord = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        // A word character is non-space
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
            if (!inWord) {
                wordCount++;
                inWord = 1;
            }
        } else {
            inWord = 0;
        }
    }

    printf("单词数量: %d\n", wordCount);

    return 0;
}
