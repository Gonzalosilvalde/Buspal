#include "busqueda.h"
#include "hashset.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <omp.h> // OpenMP

static int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

static int buscaLetraEnCadena(char *cadena, char letra) {
    while (*cadena != '\0') {
        if (tolower((unsigned char)*cadena) == tolower((unsigned char)letra))
            return 1;
        cadena++;
    }
    return 0;
}

char** buscaPalabrasConTodasLetras(char *letras, char *palabras[], int numPalabras, int *numResultados) {
    qsort(palabras, numPalabras, sizeof(char *), compare);

    char **resultados = (char**)malloc(numPalabras * sizeof(char*));
    if (resultados == NULL) {
        fprintf(stderr, "Error: no se pudo asignar memoria para los resultados.\n");
        exit(1);
    }

    int contador = 0;

    #pragma omp parallel for shared(resultados, contador)
    for (int i = 0; i < numPalabras; i++) {
        int todasEncontradas = 1;
        char *palabra = palabras[i];
        int longitud = strlen(palabra);
        int letras_len = strlen(letras);

        #pragma omp parallel for reduction(&:todasEncontradas)
        for (int j = 0; j < letras_len; j++) {
            if (!buscaLetraEnCadena(palabra, letras[j])) {
                todasEncontradas = 0;
            }
        }

        #pragma omp critical
        {
            if (todasEncontradas && letras_len == longitud) {
                resultados[contador] = strdup(palabra);
                contador++;
            }
        }
    }

    *numResultados = contador;

    return resultados;
}

char** buscaPalabrasConAlgunaLetra(char *letras, char *palabras[], int numPalabras, int *numResultados) {
    qsort(palabras, numPalabras, sizeof(char *), compare);

    char **resultados = (char**)malloc(numPalabras * sizeof(char*));
    if (resultados == NULL) {
        fprintf(stderr, "Error: no se pudo asignar memoria para los resultados.\n");
        exit(1);
    }

    int contador = 0;

    #pragma omp parallel for shared(resultados, contador)
    for (int i = 0; i < numPalabras; i++) {
        int local_contador = 0;
        char *palabra = palabras[i];
        int letras_len = strlen(letras);

        #pragma omp parallel for reduction(+:local_contador)
        for (int j = 0; j < letras_len; j++) {
            if (letras[j] != '\0' && buscaLetraEnCadena(palabra, letras[j])) {
                local_contador++;
            }
        }

        #pragma omp critical
        {
            if (local_contador > 0) {
                resultados[contador] = strdup(palabra);
                contador++;
            }
        }
    }

    *numResultados = contador;

    eliminarRepetidos(resultados, numResultados);

    return resultados;
}

char** buscaPalabrasConLetrasEnPalabra(char *letras, char *palabras[], int numPalabras, int *numResultados) {
    qsort(palabras, numPalabras, sizeof(char *), compare);

    char **resultados = (char**)malloc(numPalabras * sizeof(char*));
    if (resultados == NULL) {
        fprintf(stderr, "Error: no se pudo asignar memoria para los resultados.\n");
        exit(1);
    }

    int contador = 0;

    for (int i = 0; i < numPalabras; i++) {
        char *palabra = palabras[i];
        int letras_len = strlen(letras);
        int contieneTodas = 1;

        for (int j = 0; j < letras_len; j++) {
            if (!buscaLetraEnCadena(palabra, letras[j])) {
                contieneTodas = 0;
                break;
            }
        }

        if (contieneTodas) {
            resultados[contador] = strdup(palabra);
            contador++;
        }
    }

    *numResultados = contador;

    return resultados;
}

