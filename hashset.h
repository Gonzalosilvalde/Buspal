#ifndef HASSET_H
#define HASSET_H

#include <stdbool.h>

#define HASH_SIZE 1000000

typedef struct Node {
    char *data;
    struct Node *next;
} Node;

typedef struct {
    Node *buckets[HASH_SIZE];
} HashSet;

HashSet *createHashSet();
void insertHashSet(HashSet *set, const char *str);
void freeHashSet(HashSet *set);
void eliminarRepetidos(char **palabras, int *numResultados);

#endif // HASHSET_H

