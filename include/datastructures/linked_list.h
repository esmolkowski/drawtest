#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// Maybe add a central struct in the future
// to hold the size and point to the head and tail

typedef struct listNode {
    struct listNode *previous;
    struct listNode *next;
    void *data;
} listNode;

listNode *linked_list_create(void *data);
listNode *linked_list_add_node(listNode *node, void *data);
void linked_list_delete_node(listNode *node);

#endif