#include "Akinator.h"


const size_t LL_start_size = 500;


void   copyStringWithoutQuot(char** destination, char* str);
bool   isPositiveAnswer     (char s);
bool   isLeftChild          (AKNode* node);
size_t countCommonChars     (LinkedList* list1, LinkedList* list2);
void   writeFileNode        (AKNode* node, FILE* file);
void   addLeaf              (Tree* tree, AKNode* node);
void   suggestAnswer        (Tree* tree, AKNode* node);
void   addNodeEmpty         (Tree* tree);
void   AKnodeDestroy        (AKNode* node);
void   AKwriteGraphNode     (AKNode* node, FILE* file);
void   createChild          (AKNode** node, AKNode* parent);

void createChild(AKNode** node, AKNode* parent) {
    *node = (AKNode*)calloc(1, sizeof(AKNode));
    (*node)->parent = parent;
    (*node)->is_leaf = true;
}

void AKsay(const char* format, ...) {
    char* str_start = (char*)calloc(200, sizeof(char));
    char* out_buffer = str_start;
    va_list arguments;
    va_start(arguments, format);
    vsprintf(out_buffer, format, arguments);
    printf("%s", out_buffer);
    txSpeak("%s", out_buffer);
    free(out_buffer);
    va_end(arguments);
}

bool isPositiveAnswer(char s) {
    return (s == '+');
}

void copyStringWithoutQuot(char** destination, char* str) {
    assert(destination);
    assert(str);
    str++;
    size_t str_len = strlen(str);
    *destination = (char*)calloc(str_len, sizeof(char));
    memcpy(*destination, str, str_len);
    (*destination)[str_len - 1] = '\0';
}

AKNode* findCharacteristicsList(AKNode* node, LinkedList* list, char* entity_name) {
    assert(node);
    assert(list);
    assert(entity_name);
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
    assert(node);
    assert(entity_name);
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
    assert(node);
    assert(node->parent);
    return node->parent->left == node;
}

size_t countCommonChars(LinkedList* list1, LinkedList* list2) {
    assert(list1);
    assert(list2);
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
    assert(tree);
    assert(compare_name1);
    assert(compare_name2);
    LinkedList* list1 = NewLinkedList(LL_start_size);
    LinkedList* list2 = NewLinkedList(LL_start_size);

    AKNode* node1 = findCharacteristicsList(tree->root, list1, compare_name1);
    if (node1 == NULL) {
        AKsay("Sorry, %s not found\n", compare_name1);
        DestroyLinkedList(list1);
        DestroyLinkedList(list2);
        return;
    }

    AKNode* node2 = findCharacteristicsList(tree->root, list2, compare_name2);
    if (node2 == NULL) {
        AKsay("Sorry, %s not found\n", compare_name2);
        DestroyLinkedList(list1);
        DestroyLinkedList(list2);
        return;
    }

    assert(list1->sorted);
    assert(list2->sorted);
    size_t common_number = countCommonChars(list1, list2);
    printf("\n");
    AKsay("Both entities found\n");

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
    assert(node);
    assert(list);
    AKNode* feature = NULL;
    AKNode* last_feature = NULL;

    last_feature = LinkedListGetFront(list);
    LinkedListPopFront(list);

    char* print_buffer = (char*)calloc(200, sizeof(char));
    size_t printed_count = 0;
    printed_count += sprintf(print_buffer + printed_count, "%s is ", node->value);

    while (!(last_feature->is_leaf)) {
        feature = LinkedListGetFront(list);
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
    assert(tree);
    assert(entity_name);
    LinkedList* list = NewLinkedList(LL_start_size);
    AKNode* current_node = findCharacteristicsList(tree->root, list, entity_name);
    if (current_node == NULL) {
        AKsay("sorry, nothing found :(\n");
        return;
    }
    AKprintCharacteristicsList(current_node, list);
    DestroyLinkedList(list);
}

void AKloadFromFile(Tree* tree, const char* filename) {
    assert(tree);
    assert(filename);
    FILE* input = NULL;
    char* buffer_start = NULL;
    uint64_t buffer_size = 0;
    open_file(&input, filename, "r");
    read_buffer(&buffer_start, &buffer_size, filename, input);

    char* buffer = strtok(buffer_start, "\t\n");
    if (buffer == NULL) {
        AKsay("Sorry, database is empty\n");
        if (buffer_start != NULL) {
            free(buffer_start);
         }
        fclose(input);
        return;
    }
    char* str = NULL;
    char* new_str = NULL;

    AKNode* current_node = (AKNode*)calloc(1, sizeof(AKNode));
    current_node->value = new_str;
    current_node->is_leaf = true;
    tree->root = current_node;

    while (buffer != NULL) {
        if (*buffer == '\"') {
            str = buffer;
            new_str = str + 1;
            copyStringWithoutQuot(&new_str, str);
            current_node->value = new_str;
        } else if (*buffer == '{') {
            tree->size++;
            if (current_node->right == NULL) {
                createChild(&(current_node->right), current_node);
                current_node->is_leaf = false;
                current_node = current_node->right;
                current_node->is_leaf = true;
            } else if (current_node->left == NULL) {
                createChild(&(current_node->left), current_node);
                current_node->is_leaf = false;
                current_node = current_node->left;
                current_node->is_leaf = true;
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
        buffer = strtok(NULL, "\t\n");
    }
    free(buffer_start);
    fclose(input);
    AKsay("Database loaded\n");
}

void writeFileNode(AKNode* node, FILE* file) {
    assert(node);
    assert(file);
    fprintf(file, "\"%s\"\n", node->value);
    if (!node->is_leaf) {
        fprintf(file, "{\n");
        writeFileNode(node->right, file);
        fprintf(file, "}\n");
        fprintf(file, "{\n");
        writeFileNode(node->left, file);
        fprintf(file, "}\n");
    }
}

void AKsaveToFile(Tree* tree, const char* filename) {
    assert(tree);
    assert(filename);
    FILE* output = NULL;
    open_file(&output, filename, "w");
    writeFileNode(tree->root, output);
    fclose(output);
    AKsay("Database saved\n");
}

void AKconstructTree(Tree* tree) {
    assert(tree);
    tree->root = NULL;
    tree->size = 0;
}

void addLeaf(Tree* tree, AKNode* node) {
    assert(tree);
    assert(node);

    char* left_child_value = node->value;
    char* right_child_value = NULL;

    AKsay("So... who is it?\n");
    right_child_value = scanString();
    LinkedList* list = NewLinkedList(LL_start_size);
    AKNode* find_result = findCharacteristicsList(tree->root, list, right_child_value);
    if (find_result != NULL) {
        AKsay("You are mistaken! I already have %s in my database!\n", right_child_value);
        AKprintCharacteristicsList(find_result, list);
        DestroyLinkedList(list);
        return;
    }
    DestroyLinkedList(list);
    node->is_leaf = false;
    tree->size++;

    AKsay("What feature differs it from %s?\n", left_child_value);
    node->value = scanString();

    createChild(&(node->left), node);
    node->left->value = left_child_value;

    createChild(&(node->right), node);
    node->right->value = right_child_value;
}

void suggestAnswer(Tree* tree, AKNode* node) {
    assert(tree);
    assert(node);
    AKsay("Is it %s?\n", node->value);
    printf("(answer +/-)\n");
    char s = ' ';
    while (isspace(s)){
        s = (char)getchar();
    }
    if (isPositiveAnswer(s)) {
        AKsay("I am the most intelligent AI in the world\n");
    } else {
        AKsay("Oh no... How could I be mistaken...\n");
        addLeaf(tree, node);
        AKsay("New entity added, now i became cleverer!\n");
    }
}

char* scanString() {
    char* s = (char*)calloc(50, sizeof(char));
    size_t size = 50;
    fflush(stdin);
    size_t line_length = getline(&s, &size, stdin);
    if (s[line_length - 1] == '\n') {
        s[line_length - 1] = '\0';
    }
    return s;
}

void destroyString(char* str) {
    assert(str);
    free(str);
}

void addNodeEmpty(Tree* tree) {
    assert(tree);
    assert(tree->size == 0);
    char* s = scanString();
    tree->root = (AKNode*)calloc(1, sizeof(AKNode));
    tree->root->value = s;
    tree->root->is_leaf = true;
    tree->size = 1;
}

void AKsearch(Tree* tree) {
    assert(tree);
    if (tree->size == 0) {
        AKsay("Database is empty :-) add something!\n");
        addNodeEmpty(tree);
    } else {
        AKaskUser(tree);
    }
}

void AKaskUser(Tree* tree) {
    assert(tree);
    AKNode* node = tree->root;
    while (!node->is_leaf) {
        AKsay("%s?\n", node->value);
        printf("(answer +/-)\n");
        char s = ' ';
        while (isspace(s)){
            s = (char)getchar();
        }
        if (isPositiveAnswer(s)) {
            node = node->right;
        } else {
            node = node->left;
        }
    }
    suggestAnswer(tree, node);
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

void AKwriteGraphNode(AKNode* node, FILE* file) {
    assert(node);
    assert(file);
    if (node->is_leaf) {
        fprintf(file, "\tel%p [shape=hexagon,style=filled,fillcolor=\"#c3ff82\",label=\"%s\"];\n", node, node->value);
    } else {
        fprintf(file, "\tel%p [style=filled,fillcolor=\"#cfe8ff\",label=\"%s\"];\n", node, node->value);
        AKwriteGraphNode(node->left, file);
        AKwriteGraphNode(node->right, file);
        fprintf(file, "\tel%p -> el%p [label=\"no\" ];\n", node, node->left);
        fprintf(file, "\tel%p -> el%p [label=\"yes\"];\n", node, node->right);
    }
}

void AKmakeGraph(Tree* tree) {
    assert(tree);
    FILE* output = NULL;
    open_file(&output, "graph.txt", "w");
    fprintf(output, "digraph structs {\n\trankdir=HR;\toutputOrder=nodesfirst;\n");
    AKwriteGraphNode(tree->root, output);
    fprintf(output, "}\n");
    fclose(output);
    system("dot -Tsvg graph.txt>img.svg");
    AKsay("graph saved to %s and image saved to %s\n", "graph.txt", "img.svg");
}

void AKdestroy(Tree* tree) {
    assert(tree);
    AKnodeDestroy(tree->root);
}
