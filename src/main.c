#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void predictAndPrintAll(struct keysPredict* kt, char* partialWord ) {
	char** words;
	int wordsCount;
	words = keysPredictRun(kt, partialWord, &wordsCount);
	printf("Predicts: %s = ", partialWord);
	for(int i=0; i<wordsCount; i++) {
		printf("%s,", words[i]);
	}
	printf("\n");
	deleteArrayOfWords(words, wordsCount);
}

void findAndPrintAll(struct keysPredict* kt, char* word ) {
	struct node* result = keysPredictFind(kt, word);
	printf("Find: %s = %i\n", word, result==0);
}

/*
	Agrega al keysPredict todas las palabras pasadas por parametros

	Requiere:
		* size debe ser el tamanio que words
*/
void fillKeysPredict(struct keysPredict* kt, char** words, int size){
	for(int i = 0; i<size; i++){        
		keysPredictAddWord(kt, words[i]);
	}
}

/*
	Chequea que dos strings sean iguales

	Devuele:
		* 1 si son iguales
		* 0 si no lo son, ademas, imprimira por pantalla el error
*/
int assertStrEquals(char* val1, char* val2) {
	int i = 0;
	while (val1[i] == val2[i]){
		if(val1[i] == '\0' && val2[i] == '\0'){
			return 1;
		}
		i++;
	}
	printf("----------------- ERROR ASSERTING STR EQUALS: %s != %s -----------------\n", val1, val2);
	return 0;
}

/*
	Chequea que dos ints sean iguales

	Devuele:
		* 1 si son iguales
		* 0 si no lo son, ademas, imprimira por pantalla el error
*/
int assertIntEquals(int val1, int val2) {
	if(val1 == val2) {
		return 1;
	}
	printf("----------------- ERROR ASSERTING INT EQUALS: %i != %i -----------------\n", val1, val2);
	return 0;
}

/*
	Chequea que arreglos de strings sean iguales posicion a posicion

	Devuele:
		* 1 si son iguales
		* 0 si no lo son, ademas, imprimira por pantalla un error mostrando en que primeras
		posiciones difieren
*/
int assertArrayOfStrEquals(char** val1, char** val2, int size, int* asserted) {
	*asserted = 0;
	for(int i = 0; i<size; i++){
		*asserted += assertStrEquals(val1[i], val2[i]);
	}
	if(*asserted == size){
		return 1;
	}
	return 0;
}

/*
	Chequea que arreglos de ints sean iguales posicion a posicion

	Devuele:
		* 1 si son iguales
		* 0 si no lo son, ademas, imprimira por pantalla un error mostrando en que primeras
		posiciones difieren
*/
int assertArrayOfIntEquals(int* val1, int* val2, int size, int* asserted) {
	*asserted = 0;
	for(int i = 0; i<size; i++){
		*asserted += assertIntEquals(val1[i], val2[i]);
	}
	if(*asserted == size){
		return 1;
	}
	return 0;
}

////////// TESTS PARA strLen() ////////
void testStrLen() {
	int asserted = 0;
	int size = 3;
	int output[] = {
		strLen(""), // str vacio
		strLen("1"), // str 1 caracter
		strLen("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") // str todos los caracteres alfanumericos
	};
	int expected_output[] = {
		0,
		1,
		62
	};
	assertArrayOfIntEquals(output, expected_output, size, &asserted);
	printf("================= PASSED %i OUT OF %i TESTS FOR strLen() =================\n", asserted, size);
}

////////// TESTS PARA strDup() ////////
void testStrDup() {
	int asserted = 0;
	int size = 3;
	char* test1 = "";
	char* test2 = "1";
	char* test3 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	char* output[] = {
		strDup(test1), // str vacio
		strDup(test2), // str 1 caracter
		strDup(test3)  // str todos los caracteres alfanumericos
	};
	char* expected_output[] = {
		test1,
		test2,
		test3
	};
	assertArrayOfStrEquals(output, expected_output, size, &asserted);
	printf("================= PASSED %i OUT OF %i TESTS FOR strDup() =================\n", asserted, size);
	for (int i = 0; i < size; i++) {
		free(output[i]);
	}
}

////////// TESTS PARA keysPredict ////////

void testKeysPredict1() {
	char* words[] = {"alfajor", "canoa", "rinoceronte", "casa", "rino"}; 
	int totalWords = 5;
	int asserted = 0;
	
	struct keysPredict* kt = keysPredictNew();
	fillKeysPredict(kt, words, totalWords); // Agrega las palabras al keysPredict
	for(int i=0; i<totalWords; i++){
		struct node* n = keysPredictFind(kt, words[i]); // Busca todas las palabras agregadas
		if(n) {
			assertStrEquals(words[i], n->word); // Busca todas las palabras agregadas
			asserted++;
		}
		else {
			printf("----------------- ERROR: WORD '%s' NOT IN keysPredict -----------------\n", words[i]);
		}
	}
	printf("================= FOUND %i OUT OF %i WORDS IN keysPredict (1) =================\n", asserted, totalWords);
	
	///////////////////////////////////////////////////////////////////////////////////////
	
	keysPredictRemoveWord(kt, words[1]);
	keysPredictAddWord(kt, "pato");
	char* newWords[] = {"alfajor", "pato", "rinoceronte", "casa", "rino"}; 
	asserted = 0;
	for(int i=0; i<totalWords; i++){
		struct node* n = keysPredictFind(kt, newWords[i]); // Busca todas las palabras agregadas
		if(n) {
			assertStrEquals(newWords[i], n->word); // Busca todas las palabras agregadas
			asserted++;
		}
		else {
			printf("----------------- ERROR: WORD '%s' NOT IN keysPredict -----------------\n", words[i]);
		}
	}
	printf("================= FOUND %i OUT OF %i WORDS IN keysPredict (2) =================\n", asserted, totalWords);
	
	///////////////////////////////////////////////////////////////////////////////////////
	
	char* prefijos[] = {"c", "ca", "casa", "casas", "rino"};
	char* t1[] = {"casa"};
	char* t2[] = {"casa"};
	char* t3[] = {"casa"};
	char** t4 = 0;
	char* t5[] = {"rino", "rinoceronte"};
	char** expectedOutputs[] = {t1, t2, t3, t4, t5};
	int wordsCounts[] = {1, 1, 1, 0, 2}; // Cuantas palabras deberian ser encontradas por cada prefijo
	for(int i = 0; i<5; i++){
		int asserted = 0;
		int output_wordsCount = 0;
		char** foundWords = keysPredictRun(kt, prefijos[i], &output_wordsCount); // Busca la cantidad de palabras encontradas por cada prefijo
		assertArrayOfStrEquals(foundWords, expectedOutputs[i], wordsCounts[i], &asserted); // Chequea si se encontraron las palabras esperadas
		printf("================= FOUND %i OUT OF %i PREFIXES IN keysPredict (3) =================\n", asserted, wordsCounts[i]);
		deleteArrayOfWords(foundWords, output_wordsCount);
	}
	keysPredictDelete(kt);
}

void testKeysPredict2() {
	char* wordsKt[] = {"alfajor", "canoa", "rinoceronte", "casa", "rino"}; 
	char* wordsToFind[] = {"alfajor", "zapallo", "canoa", "telefono", "rinoceronte", "casa", "rino"}; 
	int wordsInKt[] = {1, 0, 1, 0, 1, 1, 1}; // 1 -> la palabra va a ser agregada al keysPredict, 0 -> no sera agregada
	int totalWordsInKt = 5;
	int totalWordsToFind = 7;
	int asserted = 0;
	
	struct keysPredict* kt = keysPredictNew();
	fillKeysPredict(kt, wordsKt, totalWordsInKt); // Agrega las palabras al keysPRedict
	for(int i=0; i<totalWordsToFind; i++){
		struct node* n = keysPredictFind(kt, wordsToFind[i]); // Busca las palabras en el keysPredict
		if(n) {
			assertIntEquals(wordsInKt[i], 1); // Si la encuentra, chequea que esto era lo esperado
			asserted++;
		}
		else {
			assertIntEquals(wordsInKt[i], 0); // Si no la encuentra, chequea que esto era lo esperado
		}
	}
	printf("================= FOUND %i OUT OF %i WORDS IN keysPredict (4) =================\n", asserted, totalWordsInKt);
	keysPredictDelete(kt);
}

// Codigo sacado de "predict.c" (dado en el enunciado)
int validate(char* w) {
	while(*w) {
		if(!(( 'a' <= *w && *w <= 'z' ) || *w == ' ' || *w == '-'))
			return 0;
		w++;
	}
	return 1;
}

void testKeysPredict3(){
	
	struct keysPredict* kt = keysPredictNew();
	int asserted = 0;
	
	// Codigo sacado de "predict.c" (dado en el enunciado)
	FILE *archivo = fopen("test.txt", "r");
	if (!archivo) {
		printf("Error al abrir el archivo.\n");
		return ;
	}
	char buffer[100];
	char* words[100];
	
	int i = 0;
	while (fgets(buffer, sizeof(buffer), archivo) != 0) { // Agrega al keysPredic las palabras del archivo "test.txt"
		buffer[strcspn(buffer, "\n")] = '\0';
		if(!validate(buffer)) {
			printf("Error al leer la palabra: '%s'\n", buffer);
			return;
		}
		keysPredictAddWord(kt, buffer);
		words[i] = strDup(buffer); // Agrega a un arreglo una copia de las palabras del archivo
		struct node* n = keysPredictFind(kt, words[i]);
		if(n) {
			assertStrEquals(words[i], n->word); // Chequea que se hayan agregado las palabras esperadas
			asserted++;
		}
		else {
			printf("----------------- ERROR: WORD '%s' NOT IN keysPredict -----------------\n", words[i]);
		}
		i++;
	}
	printf("================= FOUND %i OUT OF %i WORDS IN keysPredict (5) =================\n", asserted, 100);
	
	fclose(archivo);
	
	for(int i = 0; i<49; i++){
		keysPredictRemoveWord(kt, words[i]);
	}

	for (int i = 0; i<100; i++){
		free(words[i]);
	}
	
	if(assertIntEquals(kt->totalWords, 50)){ // Chequea que solo queden la mitad de las palabras iniciales
		printf("================= HALF OF THE WORDS HAVE BEEN DELETED (5) =================\n");
	}

	///////////////////////////////////////////////////////////////////////////////////////

	char* letras = "abcdefghijklmnopqrstuvwxyz";
	int wordsCount = 0;
	printf("================= PROBANDO PREFIJOS =================");
	for(int i = 0; i< strLen(letras); i++){
		for(int j = 0; j<strLen(letras); j++){
			// Crea strings de todas las combinaciones posibles de dos letras
			char prefijo[3];
			prefijo[0] = letras[i];
			prefijo[1] = letras[j];
			prefijo[2] = '\0';

			char** words = keysPredictRun(kt, prefijo, &wordsCount); // Se buscan las palabras que comiencen con el prefijo
			if(wordsCount > 0){
				printf("\nPalabras del prefijo %s: ", prefijo);
				for(int x = 0; x<wordsCount; x++){
					if(x == wordsCount -1){
						printf("%s ", words[x]);
					}
					else{
						printf("%s, ", words[x]);
					}
				}
			}
			deleteArrayOfWords(words, wordsCount);
		}
	}
	keysPredictDelete(kt);	
	printf("\n");
}

int main() {
	testStrLen();
	testStrDup();
	
	// Keys predict
	printf("\n=> RUNNING KEYSPREDICT TESTS\n");
	testKeysPredict1(); // Busca palabras y prefijos que se encuentran en el keysPredict
	testKeysPredict2(); // Busca tanto palabras que se encuentran en el keysPredict como que no
	// Crea un keysPredict de 100 palabras, le borra la mitad de las palabras y hace predicciones de prefijos de dos letras
	testKeysPredict3();  

	return 0;
}