#ifndef _UTILS_HH_
#define _UTILS_HH_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

// --- Structs ----------------------------------------------------------------

struct keysPredict {
    struct node* first;
    int totalKeys;
    int totalWords;
};

struct node {
    char character;
    struct node* next;
    int end;
    char* word;
    struct node* down;
};

// --- Keys Predict --------------------------------------------------------------

struct keysPredict* keysPredictNew();

void keysPredictAddWord(struct keysPredict* kt, char* word);

void keysPredictRemoveWord(struct keysPredict* kt, char* word);

struct node* keysPredictFind(struct keysPredict* kt, char* word);

char** keysPredictRun(struct keysPredict* kt, char* partialWord, int* wordsCount);

char** keysPredictListAll(struct keysPredict* kt, int* wordsCount);

void keysPredictDelete(struct keysPredict* kt);

void keysPredictPrint(struct keysPredict* kt);

// --- Auxiliar functions -----------------------------------------------------
    
void keysPredictPrintAux(struct node* n, int level);

struct node* findNodeInLevel(struct node** list, char character);

struct node* addSortedNewNodeInLevel(struct node** list, char character);

void deleteArrayOfWords(char** words, int wordsCount);

// --- Strings ----------------------------------------------------------------

int strLen(char* src);

char* strDup(char* src);

#endif
