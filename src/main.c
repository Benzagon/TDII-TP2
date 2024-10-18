#include <stdio.h>
#include <stdlib.h>
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

int main() {

    // COMPLETAR

    // A Continuacion dejamos algunos ejemplos de como
    // llamar a las funciones pedidas. Este codigo debe
    // ser borrado / modificado.

    // strLen
    int len = strLen("hola");
    printf("strDup(\"hola\") -> \"%i\"\n", len);
    printf("\n");

    // strDup
    char* dup = strDup("hola");
    printf("strDup(\"hola\") -> \"%s\"\n", dup);
    free(dup);
    printf("\n");

    // keysPredict
    struct keysPredict* kt = keysPredictNew();
    
    // keysPredict - crear un diccionario
    keysPredictAddWord(kt, "papanatas");
    keysPredictAddWord(kt, "zanahoria");
    keysPredictAddWord(kt, "oreja");
    keysPredictAddWord(kt, "ricardo");
    keysPredictAddWord(kt, "zaz");
    keysPredictAddWord(kt, "a");
    keysPredictAddWord(kt, "");
    keysPredictAddWord(kt, "b");
    keysPredictAddWord(kt, "ab");
    keysPredictAddWord(kt, "abfff");
    keysPredictAddWord(kt, "abfgato");
    keysPredictAddWord(kt, "bebe");
    keysPredictAddWord(kt, "pata");
    keysPredictPrint(kt);

    // keysPredict - listar todas las palabras
    char** words;
    int wordsCount;
    words = keysPredictListAll(kt, &wordsCount);
    for(int i=0; i<wordsCount; i++) {
        printf("%s\n", words[i]);
    }
    deleteArrayOfWords(words, wordsCount);

    // keysPredict - encontrar palabras
    findAndPrintAll(kt, "papa");
    findAndPrintAll(kt, "pata");
    findAndPrintAll(kt, "a");
    findAndPrintAll(kt, "zazz");

    // keysPredict - predecir palabras
    predictAndPrintAll(kt,"or");
    predictAndPrintAll(kt,"ab");
    predictAndPrintAll(kt,"pa");
    predictAndPrintAll(kt,"pap");
    predictAndPrintAll(kt,"q");
    predictAndPrintAll(kt,"zap");

    // keysPredict - predecir palabras
    keysPredictRemoveWord(kt,"");
    keysPredictRemoveWord(kt,"zaz");
    keysPredictRemoveWord(kt,"aaa");
    keysPredictRemoveWord(kt,"papa");
    keysPredictRemoveWord(kt,"pata");
    keysPredictPrint(kt);

    // keysPredict - borrar diccionario
    keysPredictDelete(kt);

    return 0;
}
