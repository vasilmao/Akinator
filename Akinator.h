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

void AKsay(const char* format, ...);
AKNode* findCharacteristicsList(AKNode* node, LinkedList* list, char* entity_name);
AKNode* findEntity(AKNode* node, char* entity_name);
void AKcompareCharateristics(Tree* tree, char* compare_name1, char* compare_name2);
void AKprintCharacteristicsList(AKNode* node, LinkedList* list);
void AKprintCharacteristics(Tree* tree, char* entity_name);
void AKloadFromFile(Tree* tree, const char* filename);
void AKsaveToFile(Tree* tree, const char* filename);
void AKconstructTree(Tree* tree);
void AKdestroy(Tree* tree);
void AKaskUser(Tree* tree);
char* scanString();
void AKsearch(Tree* tree);
void destroyString(char* str);
void AKmakeGraph(Tree* tree);
