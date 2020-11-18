#include "LinkedList.h"
#include "file_reader.h"
#include <ctype.h>

struct Node {
    char* value;
    bool is_leaf;
    Node* left;
    Node* right;
};

struct Tree {
    Node* root;
    size_t size;
};

void AKconstructTree(Tree* tree);
void AKaddLeaf(Tree* tree, Node* node);
void AKsuggestAnswer(Tree* tree, Node* node);
char* scanString();
void AKaddNodeEmpty(Tree* tree);
void AKsearch(Tree* tree);
void AKaskUser(Tree* tree, Node* node);
void AKnodeDestroy(Node* node);
void AKdestroy(Tree* tree);
void AKgetCharacteristics(Tree* tree);
void AKsave(Tree* tree);
bool isPositiveAnswer(char s);
