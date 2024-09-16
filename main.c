#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Para usar la función tolower

#define MAX_LONGITUD_PALABRA 50
#define HASH_SIZE 1000000 // Tamaño del hash set

// Función para crear y manejar el hash set de palabras
typedef struct Node 
{
    char *data;
    struct Node *next;
} Node;

typedef struct 
{
    Node *buckets[HASH_SIZE];
} HashSet;
int compare(const void *a, const void *b) 
{
    return strcmp(*(const char **)a, *(const char **)b);

}


unsigned long hash(const char *str) 
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % HASH_SIZE;
}



HashSet *createHashSet() 
{
    HashSet *set = (HashSet *)malloc(sizeof(HashSet));
    if (set == NULL) 
    {
        fprintf(stderr, "Error: No se pudo asignar memoria para el hash set.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < HASH_SIZE; ++i)
        set->buckets[i] = NULL;

    return set;
}

void insertHashSet(HashSet *set, const char *str) 
{
    unsigned long index = hash(str);
    Node *node = set->buckets[index];

    while (node != NULL) 
    {
        if (strcmp(node->data, str) == 0)
            return;
        node = node->next;
    }

    node = (Node *)malloc(sizeof(Node));
    if (node == NULL) 
    {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nodo del hash set.\n");
        exit(EXIT_FAILURE);
    }

    node->data = strdup(str);
    node->next = set->buckets[index];
    set->buckets[index] = node;
}

void freeHashSet(HashSet *set) 
{
    for (int i = 0; i < HASH_SIZE; ++i) 
    {
        Node *node = set->buckets[i];
        while (node != NULL) 
        {
            Node *temp = node;
            node = node->next;
            free(temp->data);
            free(temp);
        }
    }
    free(set);
}

void eliminarRepetidos(char **palabras, int *numResultados) 
{
    HashSet *set = createHashSet();

    int nuevosResultados = 0;
    for (int i = 0; i < *numResultados; ++i) 
    {
        if (palabras[i] != NULL) 
        {
            insertHashSet(set, palabras[i]);
            palabras[nuevosResultados++] = palabras[i];
        }
    }

    for (int i = nuevosResultados; i < *numResultados; ++i)
        palabras[i] = NULL;

    *numResultados = nuevosResultados;

    freeHashSet(set);
}
int buscaLetraEnCadena(char *cadena, char letra) 
{
    while (*cadena != '\0') 
    {
        if (tolower((unsigned char)*cadena) == tolower((unsigned char)letra))
            return 1;
        cadena++;
    }
    return 0;
}

char** buscaPalabrasConTodasLetras(char *letras, char *palabras[], int numPalabras, int *numResultados) 
{
    // Ordena el array de palabras
    qsort(palabras, numPalabras, sizeof(char *), compare);

    char **resultados = (char**)malloc(numPalabras * sizeof(char*)); // Array dinámico para almacenar los resultados
    if (resultados == NULL) 
    {
        fprintf(stderr, "Error: no se pudo asignar memoria para los resultados.\n");
        exit(1);
    }

    int contador = 0;

    // Paralelización del bucle externo
    #pragma omp parallel for shared(resultados, contador)
    for (int i = 0; i < numPalabras; i++) 
    {
        int todasEncontradas = 1;
        char *palabra = palabras[i];
        int longitud = strlen(palabra);
        int letras_len = strlen(letras); // Longitud de la cadena de letras

        // Búsqueda de letras en paralelo
        #pragma omp parallel for reduction(&:todasEncontradas)
        for (int j = 0; j < letras_len; j++) 
        {
            if (!buscaLetraEnCadena(palabra, letras[j])) 
            {
                todasEncontradas = 0;
            }
        }

        // Bloqueo crítico para actualizar resultados y contador
        #pragma omp critical
        {
            if (todasEncontradas && letras_len == longitud) 
            {
                resultados[contador] = strdup(palabra); // Copia la palabra al array de resultados
                contador++;
            }
        }
    }

    *numResultados = contador; // Actualiza el número de resultados

    return resultados;
}

char** buscaPalabrasConAlgunaLetra(char *letras, char *palabras[], int numPalabras, int *numResultados) 
{
    // Ordena el array de palabras
    qsort(palabras, numPalabras, sizeof(char *), compare);

    char **resultados = (char**)malloc(numPalabras * sizeof(char*)); // Array dinámico para almacenar los resultados
    if (resultados == NULL) 
    {
        fprintf(stderr, "Error: no se pudo asignar memoria para los resultados.\n");
        exit(1);
    }

    int contador = 0;

    // Paralelización del bucle externo
    #pragma omp parallel for shared(resultados, contador)
    for (int i = 0; i < numPalabras; i++) 
    {
        int local_contador = 0;
        char *palabra = palabras[i];
        int letras_len = strlen(letras); // Longitud de la cadena de letras

        // Búsqueda de letras en paralelo
        #pragma omp parallel for reduction(+:local_contador)
        for (int j = 0; j < letras_len; j++) 
        {
            if (letras[j] != '\0' && buscaLetraEnCadena(palabra, letras[j])) 
            {
                local_contador++;
            }
        }

        // Bloqueo crítico para actualizar resultados y contador
        #pragma omp critical
        {
            if (local_contador > 0) 
            {
                resultados[contador] = strdup(palabra); // Copia la palabra al array de resultados
                contador++;
            }
        }
    }

    *numResultados = contador; // Actualiza el número de resultados

    // Eliminar palabras repetidas
    eliminarRepetidos(resultados, numResultados);

    return resultados;
}

// Nueva función para buscar palabras que contengan todas las letras especificadas en cualquier orden y con posibles letras adicionales
char** buscaPalabrasConLetrasEnPalabra(char *letras, char *palabras[], int numPalabras, int *numResultados) 
{
    // Ordena el array de palabras
    qsort(palabras, numPalabras, sizeof(char *), compare);

    char **resultados = (char**)malloc(numPalabras * sizeof(char*)); // Array dinámico para almacenar los resultados
    if (resultados == NULL) 
    {
        fprintf(stderr, "Error: no se pudo asignar memoria para los resultados.\n");
        exit(1);
    }

    int contador = 0;

    for (int i = 0; i < numPalabras; i++) 
    {
        char *palabra = palabras[i];
        int letras_len = strlen(letras);
        int contieneTodas = 1;

        // Verifica si cada letra en 'letras' está presente en la palabra
        for (int j = 0; j < letras_len; j++) 
        {
            if (!buscaLetraEnCadena(palabra, letras[j])) 
            {
                contieneTodas = 0;
                break;
            }
        }

        if (contieneTodas) 
        {
            resultados[contador] = strdup(palabra);
            contador++;
        }
    }

    *numResultados = contador; // Actualiza el número de resultados

    return resultados;
}



int main() 
{
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
    if (archivo == NULL) 
    {
        fprintf(stderr, "Error al abrir el archivo.\n");
        return 1;
    }

    palabras = (char **)malloc(capacidadPalabras * sizeof(char *));
    while (fscanf(archivo, "%s", palabra) == 1)
    {
        if (numPalabras >= capacidadPalabras) 
        {
            capacidadPalabras *= 2;
            palabras = realloc(palabras, capacidadPalabras * sizeof(char *));
            if (palabras == NULL)
            {
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

    switch (opcion)
    {
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
    if (resultadoArchivo == NULL)
    {
        fprintf(stderr, "Error al abrir el archivo de resultados.\n");
        return 1;
    }

    fprintf(resultadoArchivo, "Palabras que ");
    if (opcion == 0)
    {
        fprintf(resultadoArchivo, "contienen todas las letras '%s':\n", letras);
    } 
    else if (opcion == 1)
    {
        fprintf(resultadoArchivo, "contienen alguna de las letras '%s':\n", letras);
    } 
    else if (opcion == 2)
    {
        fprintf(resultadoArchivo, "contienen todas las letras en cualquier orden '%s':\n", letras);
    }
    
    for (int i = 0; i < numResultados; i++)
    {
        fprintf(resultadoArchivo, "%s\n", resultados[i]);
    }

    fclose(resultadoArchivo);

    // Liberar memoria
    for (int i = 0; i < numResultados; i++)
    {
        free(resultados[i]);
    }
    free(resultados);
    free(palabras);

    return 0;
}

