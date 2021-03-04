/* list.h
*
* Created by Siyang Qiu (siyangq@student.unimelb.edu.au) 17/Aug./2020
*
* support functions for a linked list. 
* 
* functions: create new list, add elements to back of the list, search in the
  list and free list
*
*/

#ifndef LINKEDLISTH
#define LINKEDLISTH

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// record struct
typedef struct{

    char *censusYear;
    char *blockID;
    char *propertyID;
    char *basePropertyID;
    char *CLUESmallArea;
    char *tradingName;
    char *industryCode;
    char *industryDescription;
    char *xCoordinate;
    char *yCoordinate;
    char *location;

} record_t;

// linked list structs
typedef struct node node_t;

struct node {
    void *data;
    node_t *next;
};

typedef struct list list_t;

struct list {
    int num_elements;
    node_t *head;
    node_t *tail;
};

// dictionary functions
void listDelFront(list_t *list);
void listAppend(list_t *list, void *d);
list_t *newList();
void freeList(list_t *list);
void deleteRecord(record_t *rec);
int checkMatchKey(record_t *rec, char* key, FILE *filePtr);
void searchKey(list_t *list, void *key, FILE *filePtr);

// printing functions
void printNotFoundToFile(char *key, FILE *filePtr);
void printOutputToStdout(record_t *record);
void printOutputToFile(record_t *record, FILE *filePtr);
void printNotFoundToStdOut(char *key);

#endif