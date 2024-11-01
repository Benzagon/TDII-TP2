#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

/////// AUXILIARES PROPIAS ////////
/*
Crea un nodo en memoria dinamica con los valores pasados por parametro.

Devuelve: 
* El nuevo nodo.
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
	Recorre los niveles buscando la letra correspondiente al nivel por cada caracter de partialWord.
	
	Devuelve:
	* 0 si el prefijo no esta en keysPredict.
	* El ultimo nodo del prefijo si el prefijo esta en keysPredict.
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
		curr = curr->down; // Busco siguiente letra del prefijo
	}
	return prev;
}
	
/*
	A partir de un nodo, cuenta cuantas palabras existen desde ese nivel en adelante.
	
	Modifica: 
	* wordsCount: agregando la cantidad de palabras encontradas. 
	* Si el nodo pasado por parametro es NULL, no modifica wordsCount.
*/
void nodeCountAround(struct node* n, int* wordsCount){
	struct node* curr = n;
	while(curr != 0){
		if(curr->end == 1){
			(*wordsCount)++; // Sumo a wordsCount si encontre el final de una palabra.
		}
		if(curr->down != 0){ // Si existe un nivel inferior...
			nodeCountAround(curr->down, wordsCount); // Cuento todas las palabras desde ese nivel.
		}
		curr = curr->next;
	}
	return;
}

/*
	A partir de un nodo, agrega al arreglo words todas las palabras desde ese nivel en adelante.
		
	Requiere:
		* El arreglo words debe ser del mismo tamaño que la cantidad de palabras que hay desde
		ese nivel en adelante.
		* i debe ser un puntero a la primera posición a llenar del arreglo.
		* Si dupFlag es 1, las palabras en el arreglo words serán copias en memoria dinámica las palabras encontradas.
		Si duplag es 0, serán los punteros a las palabras de los nodos.
		
	Modifica:
		* En i queda la cantidad de palabras en el arreglo.
		* En words se agregan las palabras en cuestión. Si el nodo pasado por parametro es NULL,
		no modifica words.
*/
void addWordsToArray(struct node* n, char** words, int* i, int dupFlag){
	struct node* curr = n;
	while(curr != 0){
		if(curr->end == 1){
			if(dupFlag){
				words[*i] = strDup(curr->word); // Agrega a words la palabra encontrada. (Duped)
			}
			else{
				words[*i] = curr->word; // Agrega a words la palabra encontrada. (originak)
			}
			(*i)++;
		}
		if(curr->down != 0){
			addWordsToArray(curr->down, words, i, dupFlag); // Busca la palabra en el nivel de abajo.
		}
		curr = curr->next; // Busca siguiente palabra
	}
	return;
}
	
/*
	Recorre el nivel del nodo borrando (liberando la memoria) todos los niveles desde el del mismo
	hasta los ultimos.
		
	Modifica: 
		* keysPredict, liberando todo desde el nivel especificado.
*/
void abortLevel(struct node* n){
	struct node* curr = n;
	struct node* prev;
	while(curr){
		if(curr->down){
			abortLevel(curr->down); // Si el nodo posee otro abajo, borra ese primero
		}
		prev = curr;
		curr = curr->next;
		free(prev); // Borra el nodo una vez que no posee ninguno abajo
	}
	return;
}
	
/*
	Recorre los chars del arreglo src hasta encontrase con un NULL ('\0'). Este indica que finalizo
	el string, es decir, no hay mas elementos en el arreglo.

	Devuelve: la cantidad de chars del arreglo.
*/
int strLen(char* src) {
	int i = 0;
	while (src[i]!='\0'){
		i++;
	}
	return i;
}
	
/*
	Hace una copia de src, guardandola de forma dinamica. Agrega un NULL al final para indicar que
	finalizo el string.
	
	Devuelve: el puntero al nuevo string.
	
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
	Por cada letra de la palabra que se desea agregar, se agrega de forma ordenada al nivel
	corrspondiente.
	
	Modifica:
		* keysPredict, agregando a sus niveles las letras de la palabra.
		* Al nodo final de la palabra, le guarda en word la palabra y en end un 1.
*/
void keysPredictAddWord(struct keysPredict* kt, char* word) {
	int word_len = strLen(word);
	struct node** curr = &(kt->first);
	struct node* prev;
	struct node* findNode;
	for(int i = 0; i<word_len;i++){
		findNode = findNodeInLevel(curr, word[i]);
		if(!findNode){
			prev = addSortedNewNodeInLevel(curr, word[i]); // Agrega de forma ordenada el caracter al nivel correspondiente
			kt->totalKeys++;
			curr = &(prev->down); // Baja de nivel
		}
		else { // Si el nodo ya se encontraba en el nivel...
			prev = *curr;
			curr = &(findNode->down); // Baja de nivel
		}
	}
	if(word_len == 0 || prev->end == 1){
		return;
	}
	// Actualiza los valores del keysPredict
	prev->word = strDup(word);
	prev->end = 1;
	
	kt->totalWords++;
	return;
}
	
/*
	Recorre los niveles buscando el nodo final de la palabra y "borrandola".
	
	Modifica: Del nodo final de la palabra a borrar (si es que fue encontrado):
		*nodo->word, modifica el atributo word del nodo guardando un 0.
		*nodo->end, modifica el atributo end del nodo guardando un 0.
*/
void keysPredictRemoveWord(struct keysPredict* kt, char* word) {
	struct node* nodo = keysPredictFind(kt, word);
	if(!nodo) return;
	nodo->end = 0;
	free(nodo->word);
	nodo->word = 0;
	(kt->totalWords)--;
	return;
}
	
/*
	Recorre los niveles buscando el nodo final de la palabra, de caso de encontrar un nodo
	que en nodo->word tenga la palabra a buscar, significa que encontramos la palabra.
	
	Devuelve:
		* En caso de encontrar la palabra, el nodo donde se encuentra.
		* En caso de no encontrar la palabra, 0.
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
	
	if(prev->end != 1 || word_len == 0){
		return 0; // Si la palabra no esta guardada o es "".
	}
	return prev;
}
	
/*
	Dadas las primeras letras de una palabra 'partialWord', busca todas las palabras que contenga el prefijo indicado.
	
	Modifca: En wordsCount, cuantas palabras existen en los niveles por debajo del último nodo del prefijo.
	
	Devuelve: Un arreglo de tamaño wordsCount con las palabras.
	
	Obs: Deberá ser liberado luego de llamar la función.
*/
char** keysPredictRun(struct keysPredict* kt, char* partialWord, int* wordsCount) {
	int wordLen = strLen(partialWord); 
	struct node* nodo_prefijo = keysPredictFindPartialWord(kt, partialWord);
	*wordsCount = 0;
	if(nodo_prefijo == 0){ // Si el prefijo no existe en kt.
		return NULL;
	}
	
	int i = 0; // Primera posición del arreglo.
	if(nodo_prefijo->end == 1 && wordLen > 0){ // Si el prefijo es una palabra.
		(*wordsCount)++;
		i++;
	}
	
	if(wordLen > 0) { nodeCountAround(nodo_prefijo->down, wordsCount); }// Contar cuantas palabras hay debajo de partialWord.
	else { nodeCountAround(nodo_prefijo, wordsCount); }
	
	char** words = (char**) malloc(sizeof(char*) * *wordsCount);
	if(i){ // Si el prefijo es una palabra.
		words[0] = strDup(nodo_prefijo->word);
	}
	int dupFlag = 1;
	if(wordLen > 0) { addWordsToArray(nodo_prefijo->down, words, &i, dupFlag); } // Agregar a 'words' una copia de todas las palabras de la estructura.
	else { addWordsToArray(nodo_prefijo, words, &i, dupFlag); }
	return words;
}
	
/*
	Crea un arreglo en memoria dinamica con los punteros a todas las palabras almacenadas en la estructura.
	
	Modifca: En wordsCount: cuantas palabras existen en la estructura.
	
	Devuelve: Un arreglo de tamaño wordsCount con las palabras.
*/
char** keysPredictListAll(struct keysPredict* kt, int* wordsCount) {
	if(kt->first == 0){
		return NULL;
	}
	char** words = (char**) malloc(sizeof(char*) * kt->totalWords);
	int i = 0;
	int dupFlag = 0;
	addWordsToArray(kt->first, words, &i, dupFlag); // Agregar las palabras originales a 'words'.
	*wordsCount = kt->totalWords;
	
	return words;
}
	
/*
	Crea una lista con todas las palabras del keysPredict, las borra, despues borralas palabras
	de la lista y por ultimo borra el keysPredict (nodo raiz).
	
	Modifica:
		* Libera de la memoria las palabras del keysPredict.
		* Libera de la memoria todos los nodos del keysPredict.
		* Libera de la memoria el keysPredict en si.
*/
void keysPredictDelete(struct keysPredict* kt) {
	int totalWords = 0; // Borrar todas las palabras:
	char** words = keysPredictListAll(kt, &totalWords);
	deleteArrayOfWords(words, totalWords);
	
	abortLevel(kt->first); // Borrar todos los nodos:
	
	free(kt); // Borrar keysPredict:
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
}
	
// Auxiliar functions
	
/*
	Recorre la lista hasta encontrar el caracter deseado. 
	
	Devuelve:
		* Si lo encuentra, devuelve la direccion del nodo. 
		* Si no lo encuentra, devuelve 0.
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
	
/*
	Recorre el nivel hasta que encuentra un nodo cuyo caracter posee un valor de ASCII mayor al
	del caracter a agregar. Una vez que lo encuentra, lo pone anterior a este nodo encontrado,
	asi, dejando el nivel ordenado alfabeticamnete.
	
	Modifica:
		* En caso de que la lista este vacia, agrega el nodo. 
		* En caso de que la lista tenga un elemento o mas, lo agrega modificando el node->next
		de los nodos necesarios para que asi, quede el nivel ordenado.
	
	Devuelve: el nodo agregado al nivel.
*/
struct node* addSortedNewNodeInLevel(struct node** list, char character) {
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
	while(curr != 0){
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
	if(character != prev->character){
		prev->next = newNode; // newNode->next ya es 0;
		return newNode;
	}
	return prev;
}
	
/*
	Recorre la lista de palabras borrando individuaemnte cada una. Al finalizar,
	borra el arreglo en si.
	
	Modifica: words:
		* Liberando de la memoria cada palbra del arreglo.
		* Libera de la memoria el puntero del arreglo.
*/
void deleteArrayOfWords(char** words, int wordsCount) {
	int totalFreed = 0;
	for(int i = 0; i<wordsCount; i++){
		free(words[i]);
		totalFreed++;
	}
	free(words);
}
