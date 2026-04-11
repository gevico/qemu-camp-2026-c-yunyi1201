#include "mysed.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    // 检查输入参数有效性
    if (!cmd || !old_str || !new_str) {
        return -1;
    }
    
    // 初始化输出参数
    *old_str = NULL;
    *new_str = NULL;
    
    // 解析 s/old/new/ 格式
    if (cmd[0] != 's') return -1;
    const char *p = cmd + 1;
    char delim = *p++;
    if (!delim) return -1;

    // 找 old_str
    const char *old_start = p;
    while (*p && *p != delim) p++;
    if (!*p) return -1;
    *old_str = strndup(old_start, p - old_start);
    p++; // skip delim

    // 找 new_str
    const char *new_start = p;
    while (*p && *p != delim) p++;
    *new_str = strndup(new_start, p - new_start);

    return 0;
}

void replace_first_occurrence(char* str, const char* old, const char* new) {
    // 检查输入参数有效性
    if (!str || !old || !new) {
        return;
    }
    
    // 找到 old 在 str 中第一次出现的位置
    char *pos = strstr(str, old);
    if (!pos) return;

    size_t old_len = strlen(old);
    size_t new_len = strlen(new);
    // 用 memmove 腾出/收缩空间
    memmove(pos + new_len, pos + old_len, strlen(pos + old_len) + 1);
    memcpy(pos, new, new_len);
}

int __cmd_mysed(const char* rules, const char* str) {
    // 检查输入参数有效性
    if (!rules || !str) {
        fprintf(stderr, "Error: NULL rules or str parameter\n");
        return 1;
    }

    printf("rules: %s\n", rules);
    printf("str: %s\n", str);

    char* old_str = NULL;
    char* new_str = NULL;

    // 解析规则，例如 "s/old/new/"
    if (parse_replace_command(rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    if (!old_str || !new_str) {
        fprintf(stderr, "Error: Failed to parse old/new strings from rules\n");
        free(old_str);
        free(new_str);
        return 1;
    }

    // 复制原始字符串，因为我们可能会修改它（避免修改输入参数）
    char line[1024];
    strncpy(line, str, sizeof(line) - 1);
    line[sizeof(line) - 1] = '\0';  // 确保终止

    // 执行替换：在 line 中把 old_str 替换为 new_str（第一次出现）
    replace_first_occurrence(line, old_str, new_str);

    // 输出结果（建议加上换行，除非 str 本身带了）
    printf("%s\n", line);

    // 释放动态分配的内存
    free(old_str);
    free(new_str);

    return 0;
}