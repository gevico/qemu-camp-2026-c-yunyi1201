#include "circular_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

Node* create_circular_list(int n) {
    if (n <= 0) return NULL;
    Node *head = NULL, *prev = NULL;
    for (int i = 1; i <= n; i++) {
        Node *node = (Node*)malloc(sizeof(Node));
        node->id = i;
        node->next = NULL;
        if (head == NULL) {
            head = node;
        } else {
            prev->next = node;
        }
        prev = node;
    }
    // Make it circular
    prev->next = head;
    return head;
}

void free_list(Node* head) {
    if (head == NULL) return;
    Node *cur = head;
    // Break the circle first
    Node *prev = head;
    while (prev->next != head) prev = prev->next;
    prev->next = NULL;

    while (cur != NULL) {
        Node *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}
