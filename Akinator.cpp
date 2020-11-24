#include "Akinator.h"

#define CREATE_CHILD(current_node, side)                       \
    current_node->side = (AKNode*)calloc(1, sizeof(AKNode));   \
    current_node->side->parent = current_node;                 \
    current_node->side->is_leaf = true;


void copyStringWithoutQuot(char** destination, char* string);
bool isPositiveAnswer(char s);
bool isLeftChild(AKNode* node);
size_t countCommonChars(LinkedList* list1, LinkedList* list2);
void recursiveFileWrite(AKNode* node, FILE* file);
void addLeaf(Tree* tree, AKNode* node);
void suggestAnswer(Tree* tree, AKNode* node);
void addNodeEmpty(Tree* tree);
void AKnodeDestroy(AKNode* node);

void AKsay(char* format, ...) {
    char* string_start = (char*)calloc(200, sizeof(char));
    char* out_buffer = string_start;
    va_list arguments;
    va_start(arguments, format);
    vsprintf(out_buffer, format, arguments);
    printf("%s\n", out_buffer);
    txSpeak("%s", out_buffer);
    free(out_buffer);
    va_end(arguments);
}

bool isPositiveAnswer(char s) {
    return (s == '+');
}

void copyStringWithoutQuot(char** destination, char* string) {
    string++;
    size_t str_len = strlen(string);
    *destination = (char*)calloc(str_len, sizeof(char));
    memcpy(*destination, string, str_len);
    (*destination)[str_len - 1] = '\0';
}

AKNode* findCharacteristicsList(AKNode* node, LinkedList* list, char* entity_name) {
    LinkedListPushBack(list, node);
    if (node->is_leaf) {
        if (strcmp(node->value, entity_name) == 0) {
            return node;
        } else {
            LinkedListPopBack(list);
            return NULL;
        }
    }
    AKNode* result = findCharacteristicsList(node->left, list, entity_name);
    if (result != NULL) {
        return result;
    }
    result = findCharacteristicsList(node->right, list, entity_name);
    if (result != NULL) {
        return result;
    }
    LinkedListPopBack(list);
    return NULL;
}

AKNode* findEntity(AKNode* node, char* entity_name) {
    if (node->is_leaf) {
        if (strcmp(node->value, entity_name) == 0) {
            return node;
        } else {
            return NULL;
        }
    }
    AKNode* result = findEntity(node->left, entity_name);
    if (result != NULL) {
        return result;
    }
    return findEntity(node->right, entity_name);
}

bool isLeftChild(AKNode* node) {
    assert(node->parent);
    return node->parent->left == node;
}

size_t countCommonChars(LinkedList* list1, LinkedList* list2) {
    assert(list1->sorted);
    assert(list2->sorted);
    size_t common_number = 0;
    while (LinkedListGetIthLogical(list1, common_number) == LinkedListGetIthLogical(list2, common_number)) {
        ++common_number;
    }
    --common_number;
    return common_number;
}


void AKcompareCharateristics(Tree* tree, char* compare_name1, char* compare_name2) {
    LinkedList* list1 = NewLinkedList(500);
    LinkedList* list2 = NewLinkedList(500);

    AKNode* node1 = findCharacteristicsList(tree->root, list1, compare_name1);
    if (node1 == NULL) {
        AKsay("Sorry, %s not found\n", compare_name1);
        return;
    }

    AKNode* node2 = findCharacteristicsList(tree->root, list2, compare_name2);
    if (node2 == NULL) {
        AKsay("Sorry, %s not found\n", compare_name2);
        return;
    }

    assert(list1->sorted);
    assert(list2->sorted);
    size_t common_number = countCommonChars(list1, list2);

    if (common_number > 0) {
        char* print_buffer = (char*)calloc(200, sizeof(char));
        size_t printed_count = 0;
        printed_count += sprintf(print_buffer, "%s and %s are both ", node1->value, node2->value);

        for(size_t i = 0; i < common_number; ++i) {
            printed_count += sprintf(print_buffer + printed_count, "%s", LinkedListGetFront(list1)->value);
            LinkedListPopFront(list1);
            LinkedListPopFront(list2);

            if (i < common_number - 2) {
                printed_count += sprintf(print_buffer + printed_count, ", ");
            } else if (i == common_number - 2) {
                printed_count += sprintf(print_buffer + printed_count, " and ");
            } else {
                printed_count += sprintf(print_buffer + printed_count, ".\n");
            }
            printf("%s\n", print_buffer);
        }
        AKsay(print_buffer);
        free(print_buffer);
        AKsay("Now let`s talk about differences.\n");
    } else {
        AKsay("No common features.\n");
    }

    AKprintCharacteristicsList(node1, list1);
    AKprintCharacteristicsList(node2, list2);

    DestroyLinkedList(list1);
    DestroyLinkedList(list1);
}

void AKprintCharacteristicsList(AKNode* node, LinkedList* list) {
    AKNode* feature = NULL;
    AKNode* last_feature = NULL;

    last_feature = LinkedListGetFront(list);
    LinkedListPopFront(list);

    char* print_buffer = (char*)calloc(200, sizeof(char));
    size_t printed_count = 0;
    printed_count += sprintf(print_buffer + printed_count, "%s is ", node->value);

    while (!(last_feature->is_leaf)) {
        AKNode* feature = LinkedListGetFront(list);
        LinkedListPopFront(list);

        if (isLeftChild(feature)) {
            printed_count += sprintf(print_buffer + printed_count, "not %s", last_feature->value);
        } else {
            printed_count += sprintf(print_buffer + printed_count, "%s", last_feature->value);
        }

        if (list->size > 1) {
            printed_count += sprintf(print_buffer + printed_count, ", ");
        } else if (list->size == 1){
            printed_count += sprintf(print_buffer + printed_count, " and ");
        } else if (list->size == 0) {
            printed_count += sprintf(print_buffer + printed_count, ".\n");
        }

        last_feature = feature;
    }
    AKsay(print_buffer);
    free(print_buffer);
}

void AKprintCharacteristics(Tree* tree, char* entity_name) {
    LinkedList* list = NewLinkedList(500);
    AKNode* current_node = findCharacteristicsList(tree->root, list, entity_name);
    if (current_node == NULL) {
        AKsay("sorry, nothing found :(\n");
        return;
    }
    AKprintCharacteristicsList(current_node, list);
    DestroyLinkedList(list);
}

void AKloadFromFile(Tree* tree, const char* filename) {
    FILE* input = NULL;
    char* buffer_start = NULL;
    uint64_t buffer_size = 0;
    open_file(&input, filename, "r");
    read_buffer(&buffer_start, &buffer_size, filename, input);

    char* buffer = strtok(buffer_start, " \t\n\r");
    if (buffer == NULL) {
        AKsay("Sorry, database is empty\n");
        if (buffer_start != NULL) {
            free(buffer_start);
         }
        fclose(input);
        return;
    }
    char* string = NULL;
    char* new_string = NULL;

    AKNode* current_node = (AKNode*)calloc(1, sizeof(AKNode));
    current_node->value = new_string;
    current_node->is_leaf = true;
    tree->root = current_node;

    while (buffer != NULL) {
        //printf("%s\n", buffer);
        if (*buffer == '\"') {
            string = buffer;
            new_string = string + 1;
            copyStringWithoutQuot(&new_string, string);
            current_node->value = new_string;
        } else if (*buffer == '{') {
            tree->size++;
            current_node->is_leaf = false;
            if (current_node->right == NULL) {
                CREATE_CHILD(current_node, right);
                current_node = current_node->right;
            } else if (current_node->left == NULL) {
                CREATE_CHILD(current_node, left);
                current_node = current_node->left;
            } else {
                AKsay("wrong database\n");
                AKsay("more than two childs of node %s\n", current_node->value);
                assert(!"OK");
            }
        } else if (*buffer == '}') {
            current_node = current_node->parent;
        } else {
            AKsay("wrong database\n");
            AKsay("error symbol: %c\n", *buffer);
            assert(!"OK");
        }
        buffer = strtok(NULL, " \t\n\r");
    }
    //tree->root = current_node;
    free(buffer_start);
    fclose(input);
}

void recursiveFileWrite(AKNode* node, FILE* file) {
    fprintf(file, "\"%s\"\n", node->value);
    if (!node->is_leaf) {
        fprintf(file, "{\n");
        recursiveFileWrite(node->right, file);
        fprintf(file, "}\n");
        fprintf(file, "{\n");
        recursiveFileWrite(node->left, file);
        fprintf(file, "}\n");
    }
}

void AKsaveToFile(Tree* tree, const char* filename) {
    FILE* output = NULL;
    open_file(&output, filename, "w");
    recursiveFileWrite(tree->root, output);
    fclose(output);
}

void AKconstructTree(Tree* tree) {
    tree->root = NULL;
    tree->size = 0;
}

void addLeaf(Tree* tree, AKNode* node) {
    node->is_leaf = false;
    tree->size++;
    char* left_child_value = node->value;
    char* right_child_value = NULL;

    AKsay("So... who is it?\n");
    right_child_value = scanString();
    LinkedList* list = NewLinkedList(500);
    AKNode* find_result = findCharacteristicsList(tree->root, list, right_child_value);
    if (find_result != NULL) {
        AKsay("You are mistaken! I already have %s in my database!\n", right_child_value);
        AKprintCharacteristicsList(find_result, list);
        DestroyLinkedList(list);
        return;
    }

    AKsay("What feature differs it from %s?\n", left_child_value);
    node->value = scanString();

    CREATE_CHILD(node, left);
    node->left->value = left_child_value;

    CREATE_CHILD(node, right);
    node->right->value = right_child_value;
}

void suggestAnswer(Tree* tree, AKNode* node) {
    AKsay("Is it %s?\n", node->value);
    char s = ' ';
    while (isspace(s)){
        s = getchar();
    }
    //scanf(" %c", &s);
    //printf("%d\n", s);
    if (isPositiveAnswer(s)) {
        AKsay("I am the most intelligent AI in the world\n");
    } else {
        AKsay("Oh no... How could I be mistaken...\n");
        addLeaf(tree, node);
    }
}

char* scanString() {
    char* s = (char*)calloc(50, sizeof(char));
    scanf("%s", s);
    s = (char*)realloc(s, strlen(s) + 1);
    return s;
}

void destroyString(char* string) {
    free(string);
}

void addNodeEmpty(Tree* tree) {
    assert(tree);
    assert(tree->size == 0);
    char* s = scanString();
    //printf("%s\n", s);
    tree->root = (AKNode*)calloc(1, sizeof(AKNode));
    tree->root->value = s;
    tree->root->is_leaf = true;
    tree->size = 1;
}

void AKsearch(Tree* tree) {
    if (tree->size == 0) {
        AKsay("Database is empty :-) add something!\n");
        addNodeEmpty(tree);
    } else {
        AKaskUser(tree, tree->root);
    }
}

void AKaskUser(Tree* tree, AKNode* node) {
    if (node->is_leaf) {
        suggestAnswer(tree, node);
        return;
    }
    AKsay("%s?\n", node->value);
    char s = ' ';
    while (isspace(s)){
        s = getchar();
    }
    if (isPositiveAnswer(s)) {
        AKaskUser(tree, node->right);
    } else {
        AKaskUser(tree, node->left);
    }
}

void AKnodeDestroy(AKNode* node) {
    if (node == NULL) {
        return;
    }
    if (node->left != NULL) {
        AKnodeDestroy(node->left);
    }
    if (node->right != NULL) {
        AKnodeDestroy(node->right);
    }
    free(node->value);
    free(node);
}

void AKdestroy(Tree* tree) {
    AKnodeDestroy(tree->root);
}
