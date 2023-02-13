#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

extern struct Elem *dup_elem (struct Elem *head);

struct Elem
{
    struct Elem *next;
    char *str;
};

struct Elem *build_list(struct Elem *root, int len)
{
    struct Elem **cur = NULL;
    char str[10000];
    for (int i = 0; i < len; i++)
    {
        scanf("%s", str);
        char *new_str = calloc(strlen(str) + 1, sizeof(char));
        if (new_str == NULL) {
            fprintf(stderr, "Allocation failure\n");
            exit(1);
        }
        sprintf(new_str, "%s", str);
        struct Elem *new_node = calloc(1, sizeof(struct Elem));
        if (new_node == NULL) {
            fprintf(stderr, "Allocation failure\n");
            exit(1);
        }
        new_node->str = new_str;
        new_node->next = NULL;
        if (root == NULL)
        {
            root = new_node;
            cur = &root->next;
        }
        else 
        {
            *cur = new_node;
            cur = &new_node->next;
        }
    }
    return root;
}

void print_list(struct Elem *root)
{
    for (struct Elem *cur = root; cur != NULL; cur = cur->next)
        printf("%s ", cur->str);
    printf("\n");
}

void free_list(struct Elem *root)
{
    if (root != NULL) 
    {
        free(root->str);
        free_list(root->next);
    }
    free(root);
}

int main(void)
{
    struct Elem *root = NULL;
    int n;
    scanf("%d", &n);
    root = build_list(root, n);
    //print_list(root);
    root = dup_elem(root);
    print_list(root);
    free_list(root);

    return 0;
}