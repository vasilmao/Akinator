#include "LinkedList.h"

#ifdef _DEBUG
    #define ASSERT_OK {if (LinkedList_ok(list) != LISTOK) {LinkedList_dump(list); assert(!"OK");}}
#else
    #define ASSERT_OK
#endif

void   LinkedListPopSortedEnd      (struct LinkedList* list, size_t pop_index);
void   LinkedListPushSortedLinking (struct LinkedList* list, Elem_t value, size_t index);
size_t LinkedListUpdateFreeCell    (struct LinkedList* list);
void   LinkedListInsertEmpty       (struct LinkedList* list, Elem_t value);
void   freeNode                    (struct LinkedList* list, size_t free_index);


struct LinkedList* NewLinkedList(size_t capacity) {
    assert(capacity > 0);
    struct LinkedList* list = (struct LinkedList*)calloc(1, sizeof(struct LinkedList));
    list->array = (struct Node*)calloc(capacity, sizeof(struct Node));
    list->capacity = capacity;
    list->first_free = 1;
    list->size = 0;

    for (size_t i = 0; i < capacity; ++i) {
        #ifdef _DEBUG
            list->array[i].value = NAN;
        #endif
        if (i < capacity - 1 && i > 0) {
            list->array[i].next = i + 1;
        }
        if (i > 1) {
            list->array[i].prev = i - 1;
        }
    }
    list->sorted = true;
    ASSERT_OK

    return list;
}

size_t LinkedListGetPhysicalIndex(struct LinkedList* list, size_t logic_index) {
    assert(list);
    assert(0 <= logic_index && logic_index < list->size);

    ASSERT_OK

    if (list->sorted) {
        return logic_index + list->head;
    }

    if (logic_index == 0) {
        return list->head;
    }
    if (logic_index == list->size - 1) {
        return list->tail;
    }

    printf("WARNING! GETTING PHYSICAL INDEX IS SLOW OPERATION!!1!\n");
    size_t phys_i = list->head;
    for (size_t i = 0; i < logic_index; ++i) {
        phys_i = list->array[phys_i].next;
    }
    return phys_i;

    ASSERT_OK
}


Elem_t LinkedListGetIthLogical(struct LinkedList* list, size_t index) {
    assert(list);
    assert(0 <= index && index < list->size);
    ASSERT_OK

    if (list->sorted) {
        return LinkedListGetIthLogicalSorted(list, index);
    }

    return list->array[LinkedListGetPhysicalIndex(list, index)].value;
}

Elem_t LinkedListGetFront(struct LinkedList* list) {
    return LinkedListGetIthLogical(list, 0);
}

Elem_t LinkedListGetBack(struct LinkedList* list) {
    return LinkedListGetIthLogical(list, list->size - 1);
}

void LinkedListPushBack(struct LinkedList* list, Elem_t value) {
    if (list->size == 0) {
        LinkedListInsertEmpty(list, value);
    } else {
        LinkedListPushAfterPhysI(list, value, list->tail);
    }
}

void LinkedListPushFront(struct LinkedList* list, Elem_t value) {
    if (list->size == 0) {
        LinkedListInsertEmpty(list, value);
    } else {
        LinkedListPushBeforePhysI(list, value, list->head);
    }
}

void LinkedListPopBack(struct LinkedList* list) {
    LinkedListPopPhysI(list, list->tail);
}

void LinkedListPopFront(struct LinkedList* list) {
    LinkedListPopPhysI(list, list->head);
}


void LinkedListInsertEmpty(struct LinkedList* list, Elem_t value) {
    size_t push_index = list->first_free;
    list->first_free = list->array[list->first_free].next;
    list->array[list->first_free].prev = 0;
    list->array[push_index].next = 0;
    list->array[push_index].prev = 0;
    list->array[push_index].value = value;
    list->head = push_index;
    list->tail = push_index;
    list->size = 1;
}


void LinkedListPushSortedLinking(struct LinkedList* list, size_t place_to_insert) {
    assert(list);
    assert(list->size > 0);
    assert(list->sorted);

    struct Node* array = list->array;
    if (array[place_to_insert].prev != 0) {
        array[array[place_to_insert].prev].next = array[place_to_insert].next;
    }
    if (array[place_to_insert].next != 0) {
        array[array[place_to_insert].next].prev = array[place_to_insert].prev;
    }
    if (place_to_insert == list->first_free) {
        list->first_free = array[place_to_insert].next;
    }
}

size_t LinkedListUpdateFreeCell(struct LinkedList* list) {
    size_t ans = list->first_free;
    list->first_free = list->array[list->first_free].next;
    list->array[list->first_free].prev = 0;
    return ans;
}

void LinkedListPushAfterPhysI(struct LinkedList* list, Elem_t value, size_t index) {
    assert(list);
    assert(list->size > 0);
    #ifdef _DEBUG
        assert(!isnan(list->array[index].value));
    #endif

    ASSERT_OK
    size_t place_to_insert = 0;
    struct Node* array = list->array;
    if (list->sorted && index == list->tail && index < list->capacity) {
        place_to_insert = list->tail + 1;
        LinkedListPushSortedLinking(list, list->tail + 1);
    } else {
        list->sorted = false;
        place_to_insert = LinkedListUpdateFreeCell(list);
    }

    array[place_to_insert].value = value;
    array[place_to_insert].next  = list->array[index].next;
    array[place_to_insert].prev  = index;
    array[index].next            = place_to_insert;

    size_t right = array[place_to_insert].next;
    if (right != 0) {
        array[right].prev = place_to_insert;
    } else {
        list->tail = place_to_insert;
    }
    list->size++;

    ASSERT_OK
}

void LinkedListPushBeforePhysI(struct LinkedList* list, Elem_t value, size_t index) {
    assert(list);
    assert(list->size > 0);
    #ifdef _DEBUG
        assert(!isnan(list->array[index].value));
    #endif

    ASSERT_OK
    size_t place_to_insert = 0;
    struct Node* array = list->array;
    if (list->sorted && index == list->head && list->head > 1) {
        /*it is still sorted*/
        place_to_insert = list->head - 1;
        LinkedListPushSortedLinking(list, place_to_insert);
    } else {
        list->sorted = false;
        place_to_insert = LinkedListUpdateFreeCell(list);
    }

    array[place_to_insert].value = value;
    array[place_to_insert].next  = index;
    array[place_to_insert].prev  = list->array[index].prev;
    array[index].prev            = place_to_insert;

    size_t left = array[place_to_insert].prev;
    if (left != 0) {
        array[left].next = place_to_insert;
    } else {
        list->head = place_to_insert;
    }
    list->size++;

    ASSERT_OK

}


void LinkedListPopSortedEnd(struct LinkedList* list, size_t pop_index) {
    assert(list);
    #ifdef _DEBUG
        assert(!isnan(list->array[pop_index].value));
    #endif
    assert(list->sorted);

    struct Node* array = list->array;
    size_t left_i = array[pop_index].prev;
    size_t right_i = array[pop_index].next;
    if (pop_index == list->head) {
        if (pop_index == 1) {
            array[pop_index].prev  = 0;
            array[pop_index].next  = list->first_free;
            array[list->first_free].prev = pop_index;
        } else {
            array[pop_index].prev  = pop_index - 1;
            array[array[pop_index - 1].next].prev = pop_index;
            array[pop_index].next  = array[pop_index - 1].next;
            array[pop_index - 1].next = pop_index;
        }
    } else {
        array[pop_index].next = pop_index + 1;
        array[array[pop_index + 1].prev].next = pop_index;
        array[pop_index].prev = array[pop_index + 1].prev;
        array[pop_index + 1].prev = pop_index;
    }

}

void freeNode(struct LinkedList* list, size_t free_index) {
    list->array[list->first_free].prev = free_index;
    list->array[free_index].next = list->first_free;
    list->first_free = free_index;
}

void LinkedListPopPhysI(struct LinkedList* list, size_t pop_index) {
    assert(list);
    #ifdef _DEBUG
        assert(!isnan(list->array[pop_index].value));
    #endif

    ASSERT_OK

    struct Node* array = list->array;
    size_t left_i = array[pop_index].prev;
    size_t right_i = array[pop_index].next;

    if (left_i != 0) {
        array[left_i].next = right_i;
    }
    if (right_i != 0) {
        array[right_i].prev = left_i;
    }

    if (list->sorted && (pop_index == list->head || pop_index == list->tail )) {
        LinkedListPopSortedEnd(list, pop_index);
    } else {
        list->sorted = false;
        freeNode(list, pop_index);
    }

    #ifdef _DEBUG
        array[pop_index].value = NAN;
    #endif

    if (left_i == 0) {
        list->head = right_i;
    }
    if (right_i == 0) {
        list->tail = left_i;
    }

    list->size--;

    ASSERT_OK
}

//!
int LinkedList_ok(struct LinkedList* list) {
    assert(list);

    if (list->size < 0) {
        return SIZEERROR;
    }

    if (list->capacity <= 0 || list->size > list->capacity) {
        return CAPACITYERROR;
    }

    if (list->array == NULL) {
        return ARRAYPOINTERERROR;
    }

    size_t cur_ind = list->head;

    for (size_t i = 0; i < list->size; ++i) {
        if (cur_ind == 0) {
            return INDEXERRORS;
        }
        if (i == list->size - 1 && cur_ind != list->tail) {
            return LASTINDEXERROR;
        }
        if (i == list->size - 1 && list->tail != cur_ind) {
            return TAILERROR;
        }
        cur_ind = list->array[cur_ind].next;
        if (i == list->size - 1 && cur_ind != 0) {
            return INDEXERRORS;
        }
    }
    return LISTOK;
}

void LinkedList_dump(struct LinkedList* list) {
    FILE* output = fopen("listdump.txt", "w");
    assert(output);

    #define DEF_ERROR(error, number)              \
        if (result == number) {                   \
            fprintf(output, " (%s) {\n", #error); \
        } else


    fprintf(output, "LinkedList [%p]", list);
    int result = LinkedList_ok(list);
    #include "errors.h"
    /*else*/ printf("(error number %d) {\n", result);
    #undef DEF_ERROR

    fprintf(output, "\tcapacity   = %-4zu\n", list->capacity);
    fprintf(output, "\tsize       = %-4zu\n", list->size);
    fprintf(output, "\tfirst_free = %-4zu\n", list->first_free);
    fprintf(output, "\thead       = %-4zu\n", list->head);
    fprintf(output, "\ttail       = %-4zu\n", list->tail);
    fprintf(output, "\tsorted     = %-4d\n", list->sorted);
    fprintf(output, "\tarray [%p] {\n", list->array);
    for (size_t i = 0; i < list->capacity; ++i) {
        fprintf(output, "\t\t[%-3zu] = {next: %-4zu | prev: %-4zu | value: %lf}\n", i, list->array[i].next, list->array[i].prev, list->array[i].value);
    }
    fprintf(output, "\t}\n}");
    fclose(output);

    LinkedList_make_graph(list);
}


void LinkedList_make_graph(struct LinkedList* list) {
    assert(list);

    const char* start_str = "digraph structs {\n\trankdir=HR;\toutputOrder=nodesfirst;\n";
    FILE* output = fopen("graph.txt", "w");
    assert(output);
    fprintf(output, start_str);
    struct Node* array = list->array;
    for (size_t i = 0; i < list->capacity; ++i) {
        size_t last_elem = array[i].prev;
        size_t next_elem = array[i].next;
        fprintf(output, "\tel%-8zu [shape=record,label=\"{{<f0> phys pos:\\n %zu} | { <f1>prev:\\n %zu | value:\\n %lf | <f2> next:\\n %zu}}\"", i, i, last_elem, array[i].value, next_elem);
        #ifdef _DEBUG
            if (isnan(array[i].value)) {
        #else
            if (array[i].value == 0) {
        #endif
            fprintf(output, "style=filled,fillcolor=\"#ff8080\"]\n");
        } else {
            fprintf(output, "]\n");
        }
        if (i < list->capacity - 1) {
            fprintf(output, "\tel%-8zu ->el%-8zu [style=invis]\n", i, i + 1);
        }
    }
    size_t cur_elem = list->head;
    for (size_t i = 0; i < list->capacity; ++i) {

        cur_elem = i;
        size_t last_elem = array[cur_elem].prev;
        size_t next_elem = array[cur_elem].next;
        if (next_elem != 0) {
            fprintf(output, "\tel%-8zu:<f2> -> el%-8zu:<f0> [color=\"red\",constraint=false];\n", cur_elem, next_elem);
        }
        if (last_elem != 0) {
            fprintf(output, "\tel%-8zu:<f1> -> el%-8zu:<f0> [color=\"blue\"];\n", cur_elem, last_elem);
        }
        cur_elem = array[cur_elem].next;
    }
    cur_elem = list->head;
    for(size_t i = 0; i < list->size; ++i) {
        size_t last_elem = array[cur_elem].prev;
        size_t next_elem = array[cur_elem].next;
        fprintf(output, "\tellog%-5zu [shape=record,label=\"{{logical_pos: %zu |<f0> phys pos:\\n %zu} | { <f1>prev:\\n %zu | value:\\n %lf | <f2> next:\\n %zu}}\"]\n", cur_elem, i, cur_elem, last_elem, array[cur_elem].value, next_elem);
        cur_elem = next_elem;
    }
    cur_elem = list->head;
    for(size_t i = 0; i < list->size; ++i) {
        size_t last_elem = array[cur_elem].prev;
        size_t next_elem = array[cur_elem].next;
        if (next_elem != 0) {
            fprintf(output, "\tellog%-5zu:<f2> -> ellog%-5zu:<f0> [color=\"red\"];\n", cur_elem, next_elem);
        }
        if (last_elem != 0) {
            fprintf(output, "\tellog%-5zu:<f1> -> ellog%-5zu:<f0> [color=\"blue\"];\n", cur_elem, last_elem);
        }
        cur_elem = next_elem;
    }
    fprintf(output, "}");
    fclose(output);
    system("dot -Tsvg graph.txt > img.svg");
}

void LinkedListSort(struct LinkedList* list) {
    assert(list);

    ASSERT_OK

    struct Node* new_array = (struct Node*)calloc(list->capacity, sizeof(struct Node));
    assert(new_array);
    size_t cur_ind = list->head;
    #ifdef _DEBUG
        new_array[0].value = NAN;
    #endif
    for (size_t i = 0; i < list->size; ++i) {
        new_array[i + 1].value = list->array[cur_ind].value;
        new_array[i + 1].prev = i;
        new_array[i + 1].next = i + 2;
        cur_ind = list->array[cur_ind].next;
    }
    for (size_t i = list->size + 1; i < list->capacity; ++i) {
        if (i != list->capacity - 1) {
            new_array[i].next = i + 1;
        }
        #ifdef _DEBUG
            new_array[i].value = NAN;
        #endif
        if (i != list->size + 1) {
            new_array[i].prev = i - 1;
        }
    }
    new_array[list->size].next = 0;
    free(list->array);
    list->array = new_array;
    list->first_free = list->size + 1;
    list->head = 1;
    list->tail = list->size;

    list->sorted = true;

    ASSERT_OK
}

void DestroyLinkedList(struct LinkedList* list) {
    free(list->array);
    free(list);
}

Elem_t LinkedListGetIthLogicalSorted(struct LinkedList* list, size_t index) {
    assert(list);
    assert(index < list->size);
    assert(list->sorted);

    ASSERT_OK

    return list->array[index + list->head].value;
}
