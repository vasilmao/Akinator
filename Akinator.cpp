#include "Akinator.h"

#define CREATE_CHILD(current_node, side)                       \
    current_node->side = (AKNode*)calloc(1, sizeof(AKNode));   \
    current_node->side->parent = current_node;                 \
    current_node->side->is_leaf = true;


void copyStringWithoutQuot(char** destination, char* string);
bool isPositiveAnswer(char s);

bool isPositiveAnswer(char s) {
    return (s == '+');
}

void copyStringWithoutQuot(char** destination, char* string) {
    string++;
    size_t str_len = strlen(string);
    *destination = (char*)calloc(str_len, sizeof(char));
    memcpy(*destination, string, str_len);
    (*destination)[str_len - 1] = '\0';
    //printf("%s\n", *destination);
}

AKNode* findCharacteristics(AKNode* node, LinkedList* list, char* entity_name) {
    LinkedListPushBack(list, node);
    if (node->is_leaf) {
        if (strcmp(node->value, entity_name) == 0) {
            return node;
        } else {
            LinkedListPopBack(list);
            return NULL;
        }
    }
    AKNode* result = findCharacteristics(node->left, list, entity_name);
    if (result != NULL) {
        return result;
    }
    result = findCharacteristics(node->right, list, entity_name);
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

LinkedList* AKgetCharacteristics(AKNode* node) {
    assert(node->is_leaf);
    AKNode* parent = node->parent;
    LinkedList* list = NewLinkedList(500);
    LinkedListPushBack(list, node);
    while (parent != NULL) {
        LinkedListPushBack(list, parent);
        parent = parent->parent;
    }
    return list;
}

void AKcompareCharateristics(Tree* tree, char* compare_name1, char* compare_name2) {
    LinkedList* list1 = NewLinkedList(500);
    LinkedList* list2 = NewLinkedList(500);
    AKNode* node1 = findCharacteristics(tree->root, list1, compare_name1);
    if (node1 == NULL) {
        printf("Sorry, %s not found\n", compare_name1);
        return;
    }
    AKNode* node2 = findCharacteristics(tree->root, list2, compare_name2);
    if (node2 == NULL) {
        printf("Sorry, %s not found\n", compare_name2);
        return;
    }
    size_t both_features_number = 0;
    assert(list1->sorted);
    assert(list2->sorted);
    while (LinkedListGetIthLogical(list1, both_features_number) == LinkedListGetIthLogical(list2, both_features_number)) {
        ++both_features_number;
    }
    --both_features_number;
    Node* last_feature1 = NULL;
    Node* last_feature2 = NULL;
    if (both_features_number > 0) {
        printf("%s and %s are both ", node1->value, node2->value);
        for(size_t i = 0; i < both_features_number; ++i) {
            printf("%s", LinkedListGetFront(list1)->value);
            LinkedListPopFront(list1);
            LinkedListPopFront(list2);
            if (i < both_features_number - 2) {
                printf(", ");
            } else if (i == both_features_number - 2) {
                printf(" and ");
            } else {
                printf(".\n");
            }
        }
        printf("Now let`s talk about differences.\n");
    } else {
        printf("No common features.\n");
    }
    AKprintCharacteristicList(node1, list1);
    AKprintCharacteristicList(node2, list2);
    DestroyLinkedList(list1);
    DestroyLinkedList(list1);
}

void AKprintCharacteristicList(AKNode* node, LinkedList* list) {
    AKNode* feature = NULL;
    AKNode* last_feature = NULL;
    last_feature = LinkedListGetFront(list);
    LinkedListPopFront(list);
    printf("%s is ", node->value);
    while (!(last_feature->is_leaf)) {
        AKNode* feature = LinkedListGetFront(list);
        LinkedListPopFront(list);
        if (isLeftChild(feature)) {
            printf("not %s", last_feature->value);
        } else {
            printf("%s", last_feature->value);
        }
        if (list->size > 1) {
            printf(", ");
        } else if (list->size == 1){
            printf(" and ");
        } else if (list->size == 0) {
            printf(".\n");
        }
        last_feature = feature;
    }
}

void AKprintCharacteristics(Tree* tree, char* entity_name) {
    LinkedList* list = NewLinkedList(500);
    AKNode* current_node = findCharacteristics(tree->root, list, entity_name);
    if (current_node == NULL) {
        printf("sorry, nothing found :(\n");
        return;
    }
    //printf("size: %zu\n", list->size);
    //LinkedList* list_characteristics = AKgetCharacteristics(current_node);
    AKprintCharacteristicList(current_node, list);
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
        printf("Sorry, database is empty\n");
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
        printf("%s\n", buffer);
        if (*buffer == '\"') {
            string = buffer;
            new_string = NULL;
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
                printf("wrong database\n");
                printf("more than two childs of node %s\n", current_node->value);
                assert(!"OK");
            }
        } else if (*buffer == '}') {
            current_node = current_node->parent;
        } else {
            printf("wrong database\n");
            printf("error symbol: %c\n", *buffer);
            assert(!"OK");
        }
        buffer = strtok(NULL, " \t\n\r");
    }
    //tree->root = current_node;
    free(buffer_start);
    fclose(input);
}

void AKrecursiveFileWrite(AKNode* node, FILE* file) {
    fprintf(file, "\"%s\"\n", node->value);
    if (!node->is_leaf) {
        fprintf(file, "{\n");
        AKrecursiveFileWrite(node->right, file);
        fprintf(file, "}\n");
        fprintf(file, "{\n");
        AKrecursiveFileWrite(node->left, file);
        fprintf(file, "}\n");
    }
}

void AKsaveToFile(Tree* tree, const char* filename) {
    FILE* output = NULL;
    open_file(&output, filename, "w");
    AKrecursiveFileWrite(tree->root, output);
    fclose(output);
}

void AKconstructTree(Tree* tree) {
    tree->root = NULL;
    tree->size = 0;
}

void AKaddLeaf(Tree* tree, AKNode* node) {
    node->is_leaf = false;
    tree->size++;
    char* left_child_value = node->value;
    char* right_child_value = NULL;

    printf("So... who is it?\n");
    right_child_value = scanString();

    printf("What feature differs it from %s?\n", left_child_value);
    node->value = scanString();

    CREATE_CHILD(node, left);
    node->left->value = left_child_value;

    CREATE_CHILD(node, right);
    node->right->value = right_child_value;
}

void AKsuggestAnswer(Tree* tree, AKNode* node) {
    printf("Is it %s?\n", node->value);
    char s = ' ';
    while (isspace(s)){
        s = getchar();
    }
    //scanf(" %c", &s);
    //printf("%d\n", s);
    if (isPositiveAnswer(s)) {
        printf("I am the most intelligent AI in the world\n");
    } else {
        printf("Oh.. How could I be mistaken...\n");
        AKaddLeaf(tree, node);
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

void AKaddNodeEmpty(Tree* tree) {
    assert(tree);
    assert(tree->size == 0);
    char* s = scanString();
    printf("%s\n", s);
    tree->root = (AKNode*)calloc(1, sizeof(AKNode));
    tree->root->value = s;
    tree->root->is_leaf = true;
    tree->size = 1;
}

void AKsearch(Tree* tree) {
    if (tree->size == 0) {
        printf("Database is empty :-) add something!\n");
        AKaddNodeEmpty(tree);
    } else {
        AKaskUser(tree, tree->root);
    }
}

void AKaskUser(Tree* tree, AKNode* node) {
    if (node->is_leaf) {
        AKsuggestAnswer(tree, node);
        return;
    }
    printf("%s?\n", node->value);
    char s = ' ';
    while (isspace(s)){
        s = getchar();
    }
    //printf("%d %d %d\n", s, '+', '-');
    if (isPositiveAnswer(s)) {
        AKaskUser(tree, node->right);
    } else {
        AKaskUser(tree, node->left);
    }
}

void AKnodeDestroy(AKNode* node) {
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
