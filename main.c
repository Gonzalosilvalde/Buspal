#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashset.h"
#include "busqueda.h"

#define MAX_LONGITUD_PALABRA 50

int main() {
    char **palabras = NULL;
    char palabra[MAX_LONGITUD_PALABRA];
    char letras[100];
    int opcion;
    int numPalabras = 0;
    int capacidadPalabras = 10;

    printf("Ingrese las letras a buscar: ");
    scanf("%s", letras);

    printf("Ingrese 0 para buscar palabras que contengan todas las letras\n"
           "1 para buscar palabras que contengan alguna de las letras\n"
           "2 para buscar palabras que contengan todas las letras en cualquier orden\n");
    scanf("%d", &opcion);

    FILE *archivo = fopen("0_palabras_todas.txt", "r");
    if (archivo == NULL) {
        fprintf(stderr, "Error al abrir el archivo.\n");
        return 1;
    }

    palabras = (char **)malloc(capacidadPalabras * sizeof(char *));
    while (fscanf(archivo, "%s", palabra) == 1) {
        if (numPalabras >= capacidadPalabras) {
            capacidadPalabras *= 2;
            palabras = realloc(palabras, capacidadPalabras * sizeof(char *));
            if (palabras == NULL) {
                fprintf(stderr, "Error: no se pudo asignar memoria para las palabras.\n");
                exit(1);
            }
        }
        palabras[numPalabras] = strdup(palabra);
        numPalabras++;
    }

    fclose(archivo);

    int numResultados;
    char **resultados;

    switch (opcion) {
        case 0:
            resultados = buscaPalabrasConTodasLetras(letras, palabras, numPalabras, &numResultados);
            break;
        case 1:
            resultados = buscaPalabrasConAlgunaLetra(letras, palabras, numPalabras, &numResultados);
            break;
        case 2:
            resultados = buscaPalabrasConLetrasEnPalabra(letras, palabras, numPalabras, &numResultados);
            break;
        default:
            fprintf(stderr, "Opción no válida.\n");
            return 1;
    }

    FILE *resultadoArchivo = fopen("resultados.txt", "w");
    if (resultadoArchivo == NULL) {
        fprintf(stderr, "Error al abrir el archivo de resultados.\n");
        return 1;
    }

    fprintf(resultadoArchivo, "Palabras que ");
    if (opcion == 0) {
        fprintf(resultadoArchivo, "contienen todas las letras '%s':\n", letras);
    } else if (opcion == 1) {
        fprintf(resultadoArchivo, "contienen alguna de las letras '%s':\n", letras);
    } else if (opcion == 2) {
        fprintf(resultadoArchivo, "contienen todas las letras en cualquier orden '%s':\n", letras);
    }

    for (int i = 0; i < numResultados; i++) {
        fprintf(resultadoArchivo, "%s\n", resultados[i]);
    }

    fclose(resultadoArchivo);

    for (int i = 0; i < numResultados; i++) {
        free(resultados[i]);
    }
    free(resultados);
    free(palabras);

    return 0;
}

