/* list.c
*
* Created by Siyang Qiu (siyangq@student.unimelb.edu.au) 17/Aug./2020
*
* support functions for a linked list. 
* 
* functions: create new list, add elements to back of the list, search in the 
  list and free list
*
*/

#include "list.h"
#define NONE 0

/* remove node at the front of the list */
void listDelFront(list_t *list) {

    /* security checking */
    assert(list);
    assert(list->num_elements > NONE);
    assert(list->head);

    node_t *nodeToDelete = list->head;

    // free all data in the node
    deleteRecord(nodeToDelete->data);
    free(nodeToDelete->data);

    // modify the list for this change
    list->head = list->head->next;
    free(nodeToDelete);
    list->num_elements--;
    if (list->num_elements == NONE) {
        list->head = NULL;
        list->tail = NULL;
    }
}

/* add node to the back of the list */
void listAppend(list_t *list, void *d) {

    /* security checking, then create space for new node */
    assert(list);
    node_t *new = (node_t *)malloc(sizeof(node_t));
    assert(new);

    // initialise the node
    new->data = d;
    new->next = NULL;

    // add node to list
    if (list->tail) list->tail->next = new;
    list->tail = new;
    if (!(list->head)) list->head = new;
    list->num_elements++;
}

/* create a new linked list structure */
list_t *newList() {
    
    /* create new list and security checking */
    list_t *list;
    list = (list_t *)malloc(sizeof(list_t));
    assert(list);

    // initialise list
    list->head = NULL;
    list->tail = NULL;
    list->num_elements = NONE;
    return list;
}

/* free all memory associated with a list */
void freeList(list_t *list) {

    assert(list);

    // free all nodes
    while (list->num_elements) listDelFront(list);

    // free list
    free(list);
}

// search the list for a key
void searchKey(list_t *list, void *key, FILE *filePtr) {
    node_t *nodePtr = list->head;
    int keyFound = NONE;

    // linear search through whole list for the key
    while (nodePtr){
        keyFound += checkMatchKey(nodePtr->data, key, filePtr);
        nodePtr = nodePtr->next;
    }

    // if none found, print not found to file or stdout
    if (!keyFound && filePtr) printNotFoundToFile(key, filePtr);
    else if (!keyFound) printNotFoundToStdOut(key);
}