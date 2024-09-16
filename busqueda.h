#ifndef BUSQUEDA_H
#define BUSQUEDA_H

char** buscaPalabrasConTodasLetras(char *letras, char *palabras[], int numPalabras, int *numResultados);
char** buscaPalabrasConAlgunaLetra(char *letras, char *palabras[], int numPalabras, int *numResultados);
char** buscaPalabrasConLetrasEnPalabra(char *letras, char *palabras[], int numPalabras, int *numResultados);

#endif // BUSQUEDA_H

