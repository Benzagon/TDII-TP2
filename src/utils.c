#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/////// AUXILIARES PROPIAS ////////

struct node* createNode(char _character, struct node* _next, int _end, char* _word, struct node* _down){
	struct node* n = (struct node*) malloc(sizeof(struct node));
	n->character = _character;
	n->next = _next;
	n->end = _end;
	n->word = _word;
	n->down = _down;
	return n;
}

//////////////////////////////////

/*
	Recorre los chars del arreglo src hasta encontrase con un 
	NULL ('\0'). Este indica que finalizo el string, es decir, no
	hay más elementos en el arreglo. 
	Dev: la cantidad de chars del arreglo. 
*/
int strLen(char* src) {
	int i = 0;
	while (src[i]!='\0'){
		i++;
	}
	return i;
}

/*
	Copia en c (un puntero a una variable dinámica; del mismo tamaño que src) todos los caracteres
	de src. Agrega un NULL al final para indicar finalizo el string.
	Dev: el puntero al nuevo string.
	Obs: Luego de llamar la función, la memoria debe ser liberada.
*/
char* strDup(char* src) {
	int len = strLen(src);
	char* c = (char*) malloc( sizeof(char) * (len + 1) ); 
	int i = 0;
	while (src[i]!='\0'){
		c[i] = src[i];
		i++;
	}
	c[len] = '\0';
	return c;
}

// Keys Predict

struct keysPredict* keysPredictNew() {
    struct keysPredict* kt = (struct keysPredict*)malloc(sizeof(struct keysPredict));
    kt->first = 0;
    kt->totalKeys = 0;
    kt->totalWords = 0;
    return kt;
}

void keysPredictAddWord(struct keysPredict* kt, char* word) {

    // COMPLETAR

}

void keysPredictRemoveWord(struct keysPredict* kt, char* word) {

    // COMPLETAR
}

struct node* keysPredictFind(struct keysPredict* kt, char* word) {

    // COMPLETAR

    return 0;
}

char** keysPredictRun(struct keysPredict* kt, char* partialWord, int* wordsCount) {

    // COMPLETAR

    return 0;
}

int keysPredictCountWordAux(struct node* n) {

    // COMPLETAR
}

char** keysPredictListAll(struct keysPredict* kt, int* wordsCount) {

    // COMPLETAR

    return 0;
}

void keysPredictDelete(struct keysPredict* kt) {

    // COMPLETAR

}

void keysPredictPrint(struct keysPredict* kt) {
    printf("--- Predict --- Keys: %i Words: %i\n", kt->totalKeys, kt->totalWords);
    keysPredictPrintAux(kt->first, 0);
    printf("---\n");
}

void keysPredictPrintAux(struct node* n, int level) {
    if(!n) return;
    struct node* current = n;
    while(current) {
        for(int i=0; i<level;i++) printf(" |   ");
        if(current->end) {
            printf("[%c]\n",current->character);
        } else  {
            printf(" %c \n",current->character);
        }
        keysPredictPrintAux(current->down, level+1);
        current = current->next;
    }
}

// Auxiliar functions

/*
	Recorre la lista hasta encontrar el character deseado. Si lo encuentra,
	devuelve la dirección del nodo. Si no, devuelve NULL.
*/
struct node* findNodeInLevel(struct node** list, char character) {
	struct node* curr = *list;
	while(curr != 0){
		if(curr->character == character){
			return curr;
		}
		curr = curr->next;
	}
    return 0;
}

struct node* addSortedNewNodeInLevel(struct node** list, char character) { // QUE RETORNA
	struct node* newNode = createNode(character, 0, 0, 0, 0); // Función auxiliar para crear nodos;
	struct node* curr = *list;
	
	// Si list es vacía;
	if(curr == 0){
		*list = newNode;
		return newNode;
	}
	
	// Si es el primer elemento;
	else if(character < curr->character){
		*list = newNode;
		newNode->next = curr;
		return newNode;
	}
	
	// Si la lista tiene 1 solo nodo, y newNode va segundo;
	if(curr->next == 0) {
		curr->next = newNode;
		return newNode;
	}
	
	// Si tiene n>1 elementos;
	struct node* prev = curr;
	curr = curr->next;
	
	while(curr->next != 0){
		if(character < curr->character){
			newNode->next = curr;
			prev->next = newNode;
			return newNode;
		}
		else if(character == curr->character){
			return curr;
		}
		
		prev = curr;
		curr = curr->next;
	}
	
	// Verificar último elemento;
	if(character != curr->character){
		curr->next = newNode; // newNode->next ya es 0;
		return newNode;
	}
	
	return curr;
}

void deleteArrayOfWords(char** words, int wordsCount) {

    // COMPLETAR
}
