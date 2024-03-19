#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PALABRAS 1000
#define MAX_LONGITUD_PALABRA 50

// Función para comparar cadenas (necesaria para la búsqueda binaria)
int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

// Función para buscar una letra en una cadena
int buscaLetraEnCadena(const char *cadena, char letra) {
    while (*cadena != '\0') {
        if (*cadena == letra)
            return 1;
        cadena++;
    }
    return 0;
}

char** buscaPalabrasConLetra(char letra, char *palabras[], int numPalabras, int *numResultados) {
    // Ordena el array de palabras
    qsort(palabras, numPalabras, sizeof(char *), compare);

    char **resultados = (char**)malloc(numPalabras * sizeof(char*)); // Array dinámico para almacenar los resultados
    if (resultados == NULL) {
        fprintf(stderr, "Error: no se pudo asignar memoria para los resultados.\n");
        exit(1);
    }

    int contador = 0;

    // Busca la letra en cada palabra usando búsqueda binaria
    for (int i = 0; i < numPalabras; i++) {
        if (buscaLetraEnCadena(palabras[i], letra)) {
            resultados[contador] = strdup(palabras[i]); // Copia la palabra al array de resultados
            contador++;
        }
    }

    *numResultados = contador; // Actualiza el número de resultados

    return resultados;
}


int main() {
    char *palabras[MAX_PALABRAS]; // Array de punteros a las palabras
    char palabra[MAX_LONGITUD_PALABRA]; 
    char letra = 'o';
    int numPalabras = 0;

    FILE *archivo = fopen("0_palabras_todas.txt", "r");
    if (archivo == NULL) {
        fprintf(stderr, "Error al abrir el archivo.\n");
        return 1;
    }

    // Leer palabras del archivo
    while (fscanf(archivo, "%s", palabra) == 1 && numPalabras < MAX_PALABRAS) {
        palabras[numPalabras] = strdup(palabra); // Copia la palabra al array
        numPalabras++;
    }

    fclose(archivo);

    int numResultados;
    char **resultados = buscaPalabrasConLetra(letra, palabras, numPalabras, &numResultados);

    // Imprimir los resultados
    printf("Palabras que contienen la letra '%c':\n", letra);
    for (int i = 0; i < numResultados; i++) {
        printf("%s\n", resultados[i]);
    }

    // Liberar memoria
    for (int i = 0; i < numResultados; i++) {
        free(resultados[i]);
    }
    free(resultados);

    // Liberar memoria de las palabras
    for (int i = 0; i < numPalabras; i++) {
        free(palabras[i]);
    }

    return 0;
}
