#include "hashset.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % HASH_SIZE;
}

HashSet *createHashSet() {
    HashSet *set = (HashSet *)malloc(sizeof(HashSet));
    if (set == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el hash set.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < HASH_SIZE; ++i)
        set->buckets[i] = NULL;

    return set;
}

void insertHashSet(HashSet *set, const char *str) {
    unsigned long index = hash(str);
    Node *node = set->buckets[index];

    while (node != NULL) {
        if (strcmp(node->data, str) == 0)
            return;
        node = node->next;
    }

    node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nodo del hash set.\n");
        exit(EXIT_FAILURE);
    }

    node->data = strdup(str);
    node->next = set->buckets[index];
    set->buckets[index] = node;
}

void freeHashSet(HashSet *set) {
    for (int i = 0; i < HASH_SIZE; ++i) {
        Node *node = set->buckets[i];
        while (node != NULL) {
            Node *temp = node;
            node = node->next;
            free(temp->data);
            free(temp);
        }
    }
    free(set);
}

void eliminarRepetidos(char **palabras, int *numResultados) {
    HashSet *set = createHashSet();

    int nuevosResultados = 0;
    for (int i = 0; i < *numResultados; ++i) {
        if (palabras[i] != NULL) {
            insertHashSet(set, palabras[i]);
            palabras[nuevosResultados++] = palabras[i];
        }
    }

    for (int i = nuevosResultados; i < *numResultados; ++i)
        palabras[i] = NULL;

    *numResultados = nuevosResultados;

    freeHashSet(set);
}

