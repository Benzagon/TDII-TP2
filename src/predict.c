#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int validate(char* w) {
    while(*w) {
        if(!(( 'a' <= *w && *w <= 'z' ) || *w == ' ' || *w == '-'))
            return 0;
        w++;
    }
    return 1;
}

int main(int argc, char *argv[]) {

    // Mensaje de ayuda

    if (argc != 3) {
        printf("Modo de uso:\n\n");
        printf("\tPredecir:\n");
        printf("\t\t./predict.out <wordsFile> <prefix>\n\n");
        return 0;
    }

    // Lectura de parametros

    char* wordsFile = argv[1];
    char* prefix = argv[2];

    // Crear el diccionario de palabras

    struct keysPredict* kt = keysPredictNew();
    FILE *archivo = fopen(wordsFile, "r");
    if (!archivo) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), archivo) != 0) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if(!validate(buffer)) {
            printf("Error al leer la palabra: '%s'\n", buffer);
            return 1;
        }
        keysPredictAddWord(kt, buffer);
    }
    fclose(archivo);

    // Predecir

    char** words;
    int wordsCount;
    words = keysPredictRun(kt, prefix, &wordsCount);
    printf("Predicts: %s...\n", prefix);
    for(int i=0; i<wordsCount; i++) {
        printf("\t%s\n", words[i]);
    }
    deleteArrayOfWords(words, wordsCount);

    // Borrar el keysPredict

    keysPredictDelete(kt);

    return 0;
}