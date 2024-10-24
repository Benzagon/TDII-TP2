
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/////// AUXILIARES PROPIAS ////////
/*
	Crea un nodo con los valores pasados por parametro.
	Dev: el nuevo nodo.
*/
struct node* createNode(char _character, struct node* _next, int _end, char* _word, struct node* _down){
	struct node* n = (struct node*) malloc(sizeof(struct node));
	n->character = _character;
	n->next = _next;
	n->end = _end;
	n->word = _word;
	n->down = _down;
	return n;
}

/*
	Recorre los niveles buscando la letra correspondinete al nivel por cada caracter de partialWord.
	Retorna el ultimo nodo del prefijo.
	Si el prefijo no esta en keysPredict, retorna 0.
*/
struct node* keysPredictFindPartialWord(struct keysPredict* kt, char* partialWord) {
	int word_len = strLen(partialWord);
	struct node* curr = kt->first;
	struct node* prev = curr;
	for(int i = 0; i<word_len; i++){
		curr = findNodeInLevel(&curr, partialWord[i]);
		if(curr == 0){
			return 0; // Si la palabra parcial no esta.
		}
		prev = curr;
		curr = curr->down;
	}
	return prev;
}

/*
	A partir de un nodo, cuenta cuantas palabras existen desde ese nivel en adelante.
	Retorna la cantidad de palabras en wordsCount.
	Si el nodo pasado por parametro es NULL, retorna sin modificar wordsCount.
*/
void nodeCountAround(struct node* n, int* wordsCount){		
	struct node* curr = n;
	while(curr != 0){
		if(curr->end == 1){
			*wordsCount++; // Sumo si encontre el final de una palabra.
		}
		if(curr->down != 0){ // Si existe un nivel inferior.
			nodeCountAround(curr->down, wordsCount); // Cuento todas las palabras desde ese nivel.
		}
		curr = curr->next;
	}
	return;
}

/*
	A partir de un nodo, agrega a el arreglo "words" todas las palabras desde ese nivel en adelante.
	Retorna en words las palabras en cuestión.
	Si el nodo pasado por parametro es NULL, retorna sin modificar words.
	
	Requiere: 
	El arreglo words debe ser del mismo tamaño que la cantidad de palabras desde ese nivel en adelante.
	'i' debe ser la primer posición a llenar del arreglo.

	Modifica:
	'i' es la cantidad de palabras en el arreglo.
*/
void addWordsToArray(struct node* n, char** words, int i){
	struct node* curr = n;
	while(curr != 0){
		if(curr->end == 1){
			words[i] = curr->word;
			i++;
		}
		if(curr->down != 0){
			addWordsToArray(curr->down, words, i);
		}
		curr = curr->next;
	}
	return;
}

/*
	Recorre el nivel del nodo borrando (liberando la memoria) todos los niveles por debajo y el nivel del mismo nodo.
*/
void abortLevel(struct node* n){
    struct node* curr = n;
	struct node* prev;
	while(curr){
		if(curr->down){
			abortLevel(curr->down);
		}
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	return;
}
//////////////////////////////////
	
/*
	Recorre los chars del arreglo src hasta encontrase con un 
	NULL ('\0'). Este indica que finalizo el string, es decir, no
	hay mas elementos en el arreglo. 
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
	Copia en c (un puntero a una variable dinámica; del mismo tamaño 
	que src) todos los caracteres de src. Agrega un NULL al final para 
	indicar finalizo el string. 
	Dev: el puntero al nuevo string.
	Obs: Luego de llamar la funcion, la memoria debe ser liberada.
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
/*
	Por cada letra de la palabra que se desea agregar, se agrega de
	forma ordenada al nivel corrspondiente. A la ultima letra le guarda
	en word la palabra y en end guarda un 1.
*/
void keysPredictAddWord(struct keysPredict* kt, char* word) {
    int word_len = strLen(word);
	//dudoso, la idea es guardar el pointer del primer nodo del primer nivel
	struct node** curr = &(kt->first); 
	struct node* n;
	struct node* findNode;
	for(int i = 0; i<word_len;i++){
		findNode = findNodeInLevel(curr, word[i]);
		if(!findNode){
			n = addSortedNewNodeInLevel(curr, word[i]); // Agrega de forma ordenada el caracter al nivel correspondiente
			kt->totalKeys++;
			curr = &(n->down); // Bajo de nivel
		}
		else {
			curr = &(findNode->down);
		}
	}
	if(n->end == 1){
		return;
	}
	n->word = strDup(word);
	n->end = 1;
	kt->totalWords++;
	return;
}
/*
	Recorre los niveles buscando la letra correspondinete al nivel.
	Si la letra deseada es la ultima de la palabra, asigna a word
	y a end 0, asi "borrando" la palabra.
*/
void keysPredictRemoveWord(struct keysPredict* kt, char* word) {
    int word_len = strLen(word);
	struct node* curr = kt->first;
	for(int i = 0;i<word_len; i++){
		curr = findNodeInLevel(&curr, word[i]);
		if (curr->word == word){ // ESTO PUEDE FALLAR
			curr->end = 0;
			curr->word = 0;
		}
		curr = curr->down;
	}
}

// void keysPredictRemoveWord(struct keysPredict* kt, char* word) {
// 	struct node* nodo = keysPredictFind(kt, word);
//     if(!nodo) return;
// 	nodo->end = 0;
// 	nodo->word = 0;
// 	return;
// }

/*
	Recorre los niveles buscando la letra correspondinete al nivel por cada caracter de word.
	Si llega al final y la palabra esta agregada, retorna el nodo.
	En caso de no encontrar el nodo, retorna 0.
*/
struct node* keysPredictFind(struct keysPredict* kt, char* word) {
	int word_len = strLen(word);
	struct node* curr = kt->first;
	struct node* prev = curr;
	for(int i = 0; i<word_len; i++){
		curr = findNodeInLevel(&curr, word[i]);
		if(curr == 0){
			return 0; // Si la palabra no esta.
		}
		prev = curr;
		curr = curr->down;
	}
	if(prev->end != 1 && prev->word != word){
		return 0; // Si la palabra no esta guardada.
	}
	return prev;
}

/*
	Dadas las primeras letras de una palabra 'partialWord', busca todas las palabras que contenga el prefijo indicado. 
	
	Modifca: 
	En wordsCount: cuantas palabras existen en los niveles por debajo del último nodo del prefijo.
	
	Devuelve:
	Un arreglo de tamaño wordsCount con las palabras.
	Obs: Deberá ser liberado luego de llamar la función.
*/
char** keysPredictRun(struct keysPredict* kt, char* partialWord, int* wordsCount) {
	struct node* nodo_prefijo = keysPredictFindPartialWord(kt, partialWord);
	*wordsCount = 0;
    if(nodo_prefijo == 0){ // Si el prefijo no existe en kt.
		char** words = (char**) malloc(0);// ->{}
		return words; // ES LO MISMO QUE RETORNAR 0?? PREGUNTAR
	}

	int i = 0; // Primera posición del arreglo.
	if(nodo_prefijo->end == 1){ // Si el prefijo es una palabra.
		*wordsCount++;
		i++;
	}

	nodeCountAround(nodo_prefijo->down, wordsCount); // Contar cuantas palabras hay debajo de partialWord.
	char** words = (char**) malloc(sizeof(char*) * *wordsCount);	

	if(i){ // Si el prefijo es una palabra.
		words[0] = nodo_prefijo->word;
	}
	
	addWordsToArray(nodo_prefijo->down, words, i);

    return words;
}

// int keysPredictCountWordAux(struct node* n) {  ==================== QUE ES ESTO?

//     // COMPLETAR
// }

/*
	Retorna un arreglo en memoria dinamica con todas las palabras almacenadas en la estructura.
	
	Modifca: 
	En wordsCount: cuantas palabras existen en la estructura.
	
	Devuelve:
	Un arreglo de tamaño wordsCount con las palabras.
	Obs: Deberá ser liberado luego de llamar la función.
*/
char** keysPredictListAll(struct keysPredict* kt, int* wordsCount) {
	if(kt->first == 0){
		char** words = (char**) malloc(0);// ->{}
		return words; // ES LO MISMO QUE RETORNAR 0?? PREGUNTAR
	}
	char** words = (char**) malloc(sizeof(char*) * kt->totalWords);	
	addWordsToArray(kt->first, words, 0);
	*wordsCount = kt->totalWords; 
	return words;
}

/*
	Borra la estructura keysPredict completa (palabras, nodos, nodo raiz).
*/
void keysPredictDelete(struct keysPredict* kt) {
	// Borrar todas las palabras:
	int totalWords = 0;
	char** words = keysPredictListAll(kt, &totalWords);
    deleteArrayOfWords(words, totalWords);

	// Borrar todos los nodos:
	abortLevel(kt->first);

	// Borrar keysPredict:
	free(kt);
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
	Recorre la lista hasta encontrar el caracter deseado. Si lo encuentra,
	devuelve la direccion del nodo. Si no, devuelve NULL.
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
/////////////////////////////////////////////
/////////////////////////////////////////////
struct node* addSortedNewNodeInLevel(struct node** list, char character) { // QUE RETORNA ====== EL PRIMERO DEL NIVEL AAAA NOOOO NICO ME DIJO QUE ESTA BIEN
	struct node* newNode = createNode(character, 0, 0, 0, 0); // Funcion auxiliar para crear nodos;
	struct node* curr = *list;
	
	// Si list es vacia;
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
	
	// Verificar ultimo elemento;
	if(character != curr->character){
		curr->next = newNode; // newNode->next ya es 0;
		return newNode;
	}
	
	return curr;
}
///////////////////////////////////////
////////////////////////////////////////
void deleteArrayOfWords(char** words, int wordsCount) {
	for(int i = 0; i<wordsCount; i++){
		//printf("por borar %d: %s\n", i, words[i]);
		free(words[i]); //SE BORRA EL ULTIMO?
		//printf("borrado %d: %s\n", i, words[i]);
	}
	free(words);
}
/*
int main() {
	int wordsCount = 2;
	
	char** words = (char**)malloc(wordsCount * sizeof(char*));
	
	words[0] = (char*)malloc(20 * sizeof(char));  
	words[1] = (char*)malloc(20 * sizeof(char));
	//words[2] = (char*)malloc(20 * sizeof(char));
	//words[3] = (char*)malloc(20 * sizeof(char));
	
	words[0] = strDup("Hola");
	words[1] = strDup("Mundo");
	//words[2] = strDup("Wooooooooooooooooooooop");
	//words[3] = strDup("puto");
	
	for (int i = 0; i < wordsCount; i++) {
		printf("Palabra %d: %s\n", i, words[i]);
	}
	
	deleteArrayOfWords(words, wordsCount);
	for (int i = 0; i < wordsCount; i++) {
		printf("Palabra %d: %s\n", i, words[i]);
	}
	
	return 0;
}
*/
