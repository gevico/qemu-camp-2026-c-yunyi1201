// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
    // Trim leading whitespace
    char *start = str;
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
    // Trim trailing whitespace
    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = '\0';
    }
}

int load_dictionary(const char *filename, HashTable *table,
                    uint64_t *dict_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("无法打开词典文件");
    return -1;
  }

  char line[1024];
  char current_word[100] = {0};
  char current_translation[1024] = {0};
  int in_entry = 0;

  while (fgets(line, sizeof(line), file)) {
    // Remove trailing newline
    line[strcspn(line, "\n")] = '\0';

    if (line[0] == '#') {
      // New word entry: save previous if any
      if (in_entry && current_word[0] != '\0' && current_translation[0] != '\0') {
        hash_table_insert(table, current_word, current_translation);
        (*dict_count)++;
      }
      // Start new entry
      strncpy(current_word, line + 1, sizeof(current_word) - 1);
      current_word[sizeof(current_word) - 1] = '\0';
      trim(current_word);
      current_translation[0] = '\0';
      in_entry = 1;
    } else if (strncmp(line, "Trans:", 6) == 0) {
      // Translation line
      strncpy(current_translation, line + 6, sizeof(current_translation) - 1);
      current_translation[sizeof(current_translation) - 1] = '\0';
      trim(current_translation);
    }
  }

  // Insert last entry
  if (in_entry && current_word[0] != '\0' && current_translation[0] != '\0') {
    hash_table_insert(table, current_word, current_translation);
    (*dict_count)++;
  }

  fclose(file);
  return 0;
}
