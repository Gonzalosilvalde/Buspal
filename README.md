# Buscador de Palabras en C 
Este proyecto es una aplicación en C que permite buscar palabras en un archivo de texto basado en ciertas letras, con varias opciones de búsqueda. Está modularizado en diferentes archivos para facilitar su mantenimiento y comprensión.

# Funcionalidad
El programa ofrece tres tipos de búsquedas en un conjunto de palabras:

1. Palabras que contengan todas las letras especificadas.
2. Palabras que contengan alguna de las letras especificadas.
3. Palabras que contengan todas las letras especificadas en cualquier orden (y con posibles letras adicionales).
# Estructura del Proyecto
El código está dividido en varios archivos para una mejor modularidad:

- hashset.h: Declaraciones para el manejo del hash set.
- hashset.c: Implementación de las funciones del hash set. 
- busqueda.c: Implementación de las funciones de búsqueda.
- main.c: Archivo principal que utiliza las funciones definidas en los otros archivos.
# Archivos del Proyecto
### hashset.h
Define las estructuras y funciones para crear y manejar un hash set, que se utiliza para eliminar palabras repetidas.

### hashset.c
Implementa las funciones declaradas en hashset.h. Incluye la creación del hash set, la inserción de palabras, y la liberación de memoria.

### busqueda.h
Contiene las declaraciones de las funciones de búsqueda que permiten encontrar palabras en función de las letras dadas.

### busqueda.c
Implementa las funciones de búsqueda para encontrar palabras que contengan todas, alguna o todas las letras en cualquier orden.

### main.c
Contiene el código principal que interactúa con el usuario, lee las palabras desde un archivo, realiza la búsqueda según la opción seleccionada y guarda los resultados en un archivo.

## Compilación
Para compilar el proyecto, utiliza el siguiente comando:


Copiar código
`gcc -o buscador main.c hashset.c busqueda.c -fopenmp`
Esto generará un ejecutable llamado 'buscador'.

### Uso
Preparar el archivo de palabras: Asegúrate de tener un archivo llamado 0_palabras_todas.txt en el mismo directorio que el ejecutable. Este archivo debe contener una palabra por línea. En este caso hay uno en teoría con todas las palabras del castellano que encontre en el repositorio [repositorio diccionario-espanol-txt](https://github.com/JorgeDuenasLerin/diccionario-espanol-txt)

### Ejecutar el programa:

Copiar código
`./buscador`
Ingresar las letras a buscar: Cuando se te solicite, ingresa las letras que deseas buscar.

Seleccionar el tipo de búsqueda: Ingresa el número correspondiente a la opción de búsqueda:

0 para buscar palabras que contengan todas las letras.
1 para buscar palabras que contengan alguna de las letras.
2 para buscar palabras que contengan todas las letras en cualquier orden.
Revisar los resultados: Los resultados se guardarán en un archivo llamado resultados.txt.

### Dependencias
OpenMP: Para la paralelización del código de búsqueda. Asegúrate de tener OpenMP instalado y habilitado en tu compilador.




