/*
\(ᵔᵕᵔ)/
(head)->()->()->()->()->()->(tail)
*/

#include <stdio.h>
#include <stdlib.h>
#include "datastructures/linked_list.h"

listNode *linked_list_create(void *data) {
    listNode *head = malloc(sizeof(listNode));
    head->data = data;
}

listNode *linked_list_add_node(listNode *node, void *data) {
    /*
        adds new data to the linked list

        inputs:
        node - preferably the tail node, but can be any
        data - data to create new node with
    */
    if (node->next != NULL) {
        while (node->next != NULL) {
            node = node->next;
        }
    }
    listNode *newNode = malloc(sizeof(listNode));
    node->next = newNode;
    newNode->previous = node;
    newNode->data = data;
    return newNode;
}

void linked_list_delete_node(listNode *node) {
    /*
        deletes a node
        can be anywhere
        DOES NOT DELETE THE DATA
        
        inputs:
        node - the node to delete :)
    */

    // The previous and next nodes for this current node
    listNode *nPrevious = node->previous;
    listNode *nNext = node->next;

    if (nPrevious == NULL) {
        // we are deleting the head
        // is this the right way to do this??
        nNext->previous = NULL;
    } else if (nNext == NULL) {
        // we are deleting the head
        nPrevious->next = NULL;
    } else {
        nNext->previous = node->previous;
        nPrevious->next = node->next;
    }
    free(node);
}

/*
figure out how to handle deleting the entire LL later
if it just went through all at once how would the data get deleted
void linked_list_delete(listNode *node) {
    /*
        deletes the entire linked list
        can be anywhere but head is ideal 
        DOES NOT DELETE THE DATA
        
        inputs:
        node - any node, but head is best
    /
    
    while (node->previous != NULL) {
        node = head->previous;
    }
    free(node);
}
*/