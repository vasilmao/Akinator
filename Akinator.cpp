#include "Akinator.h"

bool isPositiveAnswer(char s) {
    return (s == '+');
}

void AKsave(Tree* tree) {
    return;
}

void AKgetCharacteristics(Tree* tree) {
    return;
}

void AKconstructTree(Tree* tree) {
    tree->root = NULL;
    tree->size = 0;
}

void AKaddLeaf(Tree* tree, Node* node) {
    node->is_leaf = false;
    tree->size++;
    char* left_child_value = node->value;
    char* right_child_value = NULL;

    printf("So... who is it?\n");
    right_child_value = scanString();

    printf("What feature differs it from %s?\n", left_child_value);
    node->value = scanString();

    node->left = (Node*)calloc(1, sizeof(Node));
    node->left->value = left_child_value;
    node->left->is_leaf = true;

    node->right = (Node*)calloc(1, sizeof(Node));
    node->right->value = right_child_value;
    node->right->is_leaf = true;
}

void AKsuggestAnswer(Tree* tree, Node* node) {
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

void AKaddNodeEmpty(Tree* tree) {
    assert(tree);
    assert(tree->size == 0);
    char* s = scanString();
    printf("%s\n", s);
    tree->root = (Node*)calloc(1, sizeof(Node));
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

void AKaskUser(Tree* tree, Node* node) {
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

void AKnodeDestroy(Node* node) {
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
