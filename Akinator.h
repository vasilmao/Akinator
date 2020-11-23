#pragma once
#include "file_reader.h"
#include <windows.h>
#include <Servprov.h>
#define TX_USE_SPEAK
#include "TXLib.h"
#include <ctype.h>

struct AKNode {
    char* value;
    bool is_leaf;
    AKNode* left;
    AKNode* right;
    AKNode* parent;
};

#include "LinkedList.h"

struct Tree {
    AKNode* root;
    size_t size;
};


void AKconstructTree(Tree* tree);
void AKaddLeaf(Tree* tree, AKNode* node);
void AKsuggestAnswer(Tree* tree, AKNode* node);
char* scanString();
void  destroyString(char* string);
void AKaddNodeEmpty(Tree* tree);
void AKsearch(Tree* tree);
void AKaskUser(Tree* tree, AKNode* node);
void AKnodeDestroy(AKNode* node);
void AKdestroy(Tree* tree);
void AKprintCharacteristics(Tree* tree, char* entity_name);
void AKprintCharacteristicList(AKNode* node, LinkedList* list);
void AKcompareCharateristics(Tree* tree, char* compare_name1, char* compare_name2);
void AKsaveToFile(Tree* tree, const char* filename);
void AKloadFromFile(Tree* tree, const char* filename);
