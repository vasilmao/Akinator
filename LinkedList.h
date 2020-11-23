#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

struct AKNode;

typedef AKNode* Elem_t;

#define DEF_ERROR(name, number) name = number,

enum ListErrors {
    #include "errors.h"
};

#undef DEF_ERROR

struct Node {
    size_t next;
    size_t prev;
    Elem_t value;
};

struct LinkedList {
    Node* array;
    size_t capacity;
    size_t first_free;
    size_t head;
    size_t tail;
    size_t size;
    bool sorted;
};

struct LinkedList* NewLinkedList     (size_t capacity);
Elem_t LinkedListGetIthLogical       (struct LinkedList* list, size_t index);
Elem_t LinkedListGetFront            (struct LinkedList* list);
Elem_t LinkedListGetBack             (struct LinkedList* list);
size_t LinkedListGetPhysicalIndex    (struct LinkedList* list, size_t index);
void   LinkedListPushBack            (struct LinkedList* list, Elem_t value);
void   LinkedListPushFront           (struct LinkedList* list, Elem_t value);
void   LinkedListPushAfterPhysI      (struct LinkedList* list, Elem_t value, size_t index);
void   LinkedListPushBeforePhysI     (struct LinkedList* list, Elem_t value, size_t index);
void   LinkedListPopBack             (struct LinkedList* list);
void   LinkedListPopFront            (struct LinkedList* list);
void   LinkedListPopPhysI            (struct LinkedList* list, size_t pop_index);
void   LinkedList_make_graph         (struct LinkedList* list);
void   LinkedList_dump               (struct LinkedList* list);
int    LinkedList_ok                 (struct LinkedList* list);
void   LinkedListSort                (struct LinkedList* list);
Elem_t LinkedListGetIthLogicalSorted (struct LinkedList* list, size_t index);
void   DestroyLinkedList             (struct LinkedList* list);
