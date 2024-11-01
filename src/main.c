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

void fillKeysPredict(struct keysPredict* kt, char** words, int size){
	for(int i = 0; i<size; i++){        
		keysPredictAddWord(kt, words[i]);
	}
}
	
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
	
	int assertIntEquals(int val1, int val2) {
		if(val1 == val2) {
			return 1;
		}
		printf("----------------- ERROR ASSERTING INT EQUALS: %i != %i -----------------\n", val1, val2);
		return 0;
	}
	
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
		fillKeysPredict(kt, words, totalWords);
		for(int i=0; i<totalWords; i++){
			struct node* n = keysPredictFind(kt, words[i]);
			if(n) {
				assertStrEquals(words[i], n->word);
				asserted++;
			}
			else {
				printf("----------------- ERROR: WORD '%s' NOT IN keysPredict -----------------\n", words[i]);
			}
		}
		printf("================= FOUND %i OUT OF %i WORDS IN keysPredict (1) =================\n", asserted, totalWords);
		
		/////////////////////////////////////////////////////
		
		keysPredictRemoveWord(kt, words[1]);
		keysPredictAddWord(kt, "pato");
		char* newWords[] = {"alfajor", "pato", "rinoceronte", "casa", "rino"}; 
		asserted = 0;
		for(int i=0; i<totalWords; i++){
			struct node* n = keysPredictFind(kt, newWords[i]);
			if(n) {
				assertStrEquals(newWords[i], n->word);
				asserted++;
			}
			else {
				printf("----------------- ERROR: WORD '%s' NOT IN keysPredict -----------------\n", words[i]);
			}
		}
		printf("================= FOUND %i OUT OF %i WORDS IN keysPredict (2) =================\n", asserted, totalWords);
		
		////////////////////////////////////////////////////////
		
		char* prefijos[] = {"c", "ca", "casa", "casas", "rino"};
		char* t1[] = {"casa"};
		char* t2[] = {"casa"};
		char* t3[] = {"casa"};
		char** t4 = 0;
		char* t5[] = {"rino", "rinoceronte"};
		char** expectedOutputs[] = {t1, t2, t3, t4, t5};
		int wordsCounts[] = {1, 1, 1, 0, 2};
		for(int i = 0; i<5; i++){
			int asserted = 0;
			int output_wordsCount = 0;
			char** foundWords = keysPredictRun(kt, prefijos[i], &output_wordsCount);
			assertArrayOfStrEquals(foundWords, expectedOutputs[i], wordsCounts[i], &asserted);
			printf("================= FOUND %i OUT OF %i PREFIXES IN keysPredict (3) =================\n", asserted, wordsCounts[i]);
			deleteArrayOfWords(foundWords, output_wordsCount);
		}
		keysPredictDelete(kt);
	}
	
	void testKeysPredict2() {
		char* wordsKt[] = {"alfajor", "canoa", "rinoceronte", "casa", "rino"}; 
		char* wordsToFind[] = {"alfajor", "zapallo", "canoa", "telefono", "rinoceronte", "casa", "rino"}; 
		int wordsInKt[] = {1, 0, 1, 0, 1, 1, 1};
		int totalWordsInKt = 5;
		int totalWordsToFind = 7;
		int asserted = 0;
		
		struct keysPredict* kt = keysPredictNew();
		fillKeysPredict(kt, wordsKt, totalWordsInKt);
		for(int i=0; i<totalWordsToFind; i++){
			struct node* n = keysPredictFind(kt, wordsToFind[i]);
			if(n) {
				assertIntEquals(wordsInKt[i], 1);
				asserted++;
			}
			else {
				assertIntEquals(wordsInKt[i], 0);
			}
		}
		printf("================= FOUND %i OUT OF %i WORDS IN keysPredict (4) =================\n", asserted, totalWordsInKt);
		keysPredictDelete(kt);
	}
	
	// keysPredict casos grandes.
	// 1. Armar un diccionario con 100 palabras cualesquiera.
	// 2. Borrar la mitad de las palabras del diccionario.
	// 3. Predecir todas las combinaciones posibles de prefijos dos letras.
	
	//funcion robada
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
		
		//codigo robado
		FILE *archivo = fopen("test.txt", "r");
		if (!archivo) {
			printf("Error al abrir el archivo.\n");
			return ;
		}
		char buffer[100];
		char* words[100];
		
		int i = 0;
		while (fgets(buffer, sizeof(buffer), archivo) != 0) {
			buffer[strcspn(buffer, "\n")] = '\0';
			if(!validate(buffer)) {
				printf("Error al leer la palabra: '%s'\n", buffer);
				return ;
			}
			keysPredictAddWord(kt, buffer);
			words[i] = strDup(buffer);
			// printf("%s && i: %i\n", words[1], i);
			struct node* n = keysPredictFind(kt, words[i]);
			if(n) {
				assertStrEquals(words[i], n->word);
				asserted++;
			}
			else {
				printf("----------------- ERROR: WORD '%s' NOT IN keysPredict -----------------\n", words[i]);
			}
			
			i++;
		}
		printf("================= FOUND %i OUT OF %i WORDS IN keysPredict (5) =================\n", asserted, 100);
		
		fclose(archivo);
		
		//FILE *wop = fopen("test.txt", "r");
		// i = 0;
		// while (fgets(buffer, sizeof(buffer), wop) != 0 && i < 49) {
		for(int i = 0; i<49; i++){
			keysPredictRemoveWord(kt, words[i]);
			
		}
		//buffer[strcspn(buffer, "\n")] = '\0';
		
		
		//i++;
		//}
		
		//fclose(wop);
		
		if(assertIntEquals(kt->totalWords, 50)){
			printf("================= HALF OF THE WORDS HAVE BEEN DELETED (5) =================\n");
		}
	}
		
		
		int main() {
			testStrLen();
			testStrDup();
			
			// Keys predict
			printf("\n=> RUNNING KEYSPREDICT TESTS\n");
			testKeysPredict1(); // Busca palabras y prefijos que se encuentran en el keysPredict
			// testKeysPredict2(); // Busca tanto palabras que se encuentran en el keysPredict como que no
			testKeysPredict3(); 
			
			
			
			// COMPLETAR
			
			// A Continuacion dejamos algunos ejemplos de como
			// llamar a las funciones pedidas. Este codigo debe
			// ser borrado / modificado.
			
			// strLen
			// int len = strLen("hola");
			// printf("strLen(\"hola\") -> \"%i\"\n", len);
			// printf("\n");
			
			// // strDup
			// char* dup = strDup("hola");
			// printf("strDup(\"hola\") -> \"%s\"\n", dup);
			// free(dup);
			// printf("\n");
			
			//keysPredict
			// struct keysPredict* kt = keysPredictNew();
			
			//keysPredict - crear un diccionario
			// keysPredictAddWord(kt, "zapallo");
			// keysPredictAddWord(kt, "como");
			// keysPredictAddWord(kt, "su");
			// keysPredictAddWord(kt, "que");
			// keysPredictAddWord(kt, "el");
			// keysPredictPrint(kt);
			
			// keysPredict - listar todas las palabras ============ PROBADA
			// char** words;
			// int wordsCount;
			// words = keysPredictListAll(kt, &wordsCount);
			
			// for(int i=0; i<wordsCount; i++) {
			//     printf("%s\n", words[i]);
			// }
			// deleteArrayOfWords(words, wordsCount);
			// keysPredict - encontrar palabras ============ PROBADA
			// findAndPrintAll(kt, "papa");
			// findAndPrintAll(kt, "pata");
			// findAndPrintAll(kt, "a");
			// findAndPrintAll(kt, "zazz");
			
			// keysPredict - predecir palabras ============ PROBADA
			//predictAndPrintAll(kt,"e");
			// predictAndPrintAll(kt,"ab");
			// predictAndPrintAll(kt,"pa");
			// predictAndPrintAll(kt,"weird");
			// predictAndPrintAll(kt,"q");
			// predictAndPrintAll(kt,"zap");
			
			// keysPredict - predecir palabras ============== PROBADA
			// keysPredictRemoveWord(kt,"");
			// keysPredictRemoveWord(kt,"zaz");
			// keysPredictRemoveWord(kt,"aaa");
			// keysPredictRemoveWord(kt,"papa");
			// keysPredictRemoveWord(kt,"pata");
			// keysPredictRemoveWord(kt,"zanahoria");
			// keysPredictPrint(kt);
			
			// keysPredict - borrar diccionario
			// //keysPredictDelete(kt);
			
			return 0;
		}
		
