#include <stdio.h>
#include <stdlib.h>

#include "circular_linked_list.h"

// 使用环形链表模拟约瑟夫环并打印出列顺序
static void josephus_problem(int n, int k, int m) {
    if (n <= 0 || k <= 0 || m <= 0) {
        printf("参数错误\n");
        return;
    }

    Node* head = create_circular_list(n);
    if (!head) {
        printf("\n");
        return;
    }

    // prev 指向 current 的前一个节点
    Node* current = head;
    Node* prev = head;
    while (prev->next != head) prev = prev->next;

    // 起始位置移动到第 k 个
    for (int i = 1; i < k; ++i) {
        prev = current;
        current = current->next;
    }

    // 模拟约瑟夫环，依次删除节点
    int remaining = n;
    while (remaining > 0) {
        // 从current开始数m步
        for (int step = 1; step < m; step++) {
            prev = current;
            current = current->next;
        }
        // current 是要出列的人
        printf("%d ", current->id);
        remaining--;

        if (remaining == 0) {
            free(current);
            break;
        }

        // Remove current from circle
        Node *to_free = current;
        prev->next = current->next;
        current = current->next;
        free(to_free);
    }

    printf("\n");
}

int main(void) {
    josephus_problem(5, 1, 2);  // 输出结果：2 4 1 5 3
    josephus_problem(7, 3, 1);  // 输出结果：3 4 5 6 7 1 2
    josephus_problem(9, 1, 8);  // 输出结果：8 7 9 2 5 4 1 6 3
    return 0;
}
