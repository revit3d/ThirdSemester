#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

struct Elem
{
    struct Elem *next;
    char *str;
};

int to_integer (const char *str, int *int_len, int *success_flag) {
    if (str == NULL) {
        *success_flag = 0;
        return 0;
    }
    int cur_pos = 0;
    int result = 0;
    int sign = 1;
    while (str[cur_pos] == ' ')
        cur_pos++;
    if (str[cur_pos] == '-') {
        sign = -1;
    } else if (str[cur_pos] == '+') {
        sign = 1;
    } else if (str[cur_pos] >= '0' && str[cur_pos] <= '9') {
        cur_pos--;
    } else {
        *success_flag = 0;
        return 0;
    }
    cur_pos++;
    while (str[cur_pos] != '\0') {
        if (str[cur_pos] >= '0' && str[cur_pos] <= '9') {
            if (result != 0)
                *int_len += 1;
            if (__builtin_mul_overflow(result, 10, &result)
                || __builtin_add_overflow(result, sign * (str[cur_pos] - '0'), &result)) {
                    *success_flag = 0;
                    return 0;
                }
        } else {
            *success_flag = 0;
            return 0;
        }
        cur_pos++;
    }
    if (result < 0)
        *int_len += 1;
    return result;
}

struct Elem *
dup_elem (struct Elem *head) {
    if (head == NULL)
        return NULL;
    struct Elem *root = head;
    struct Elem *prev = head;
    while (head != NULL) {
        int success_flag = 1;
        int int_len = 1;
        int num = to_integer(head->str, &int_len, &success_flag);
        if (!success_flag
            || __builtin_add_overflow(num, 1, &num)) {
            if (head == prev) {
                head = head->next;
            } else {
                head = head->next;
                prev = prev->next;
            }
            continue;
        }
        char *new_str = calloc(int_len + 1, sizeof(char));
        if (new_str == NULL) {
            fprintf(stderr, "Allocation failure\n");
            exit(1);
        }
        sprintf(new_str, "%d", num);
        struct Elem *new_node = calloc(1, sizeof(struct Elem));
        if (new_node == NULL) {
            fprintf(stderr, "Allocation failure\n");
            exit(1);
        }
        new_node->next = head;
        new_node->str = new_str;
        if (head != prev) {
            prev->next = new_node;
            prev = head;
            head = head->next;
        } else {
            head = head->next;
            root = new_node;
        }
    }
    return root;
}