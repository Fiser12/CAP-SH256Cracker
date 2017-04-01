#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mypow.h"
#include "hasher/sha256.h"

/* Programa que dado un ALFABETO, un MINIMO y un MAXIMO de longitud de clave y el DIGGEST (generado por un hasher), genera una lista con tadas las claves candidatas posibles al mismo tiempo que contrasta si el diggest coincide con el hash de la clave candidata. Si coincide o no, se muestra un mensaje por pantalla indicandolo. */
/* ------------------------------------------------------ */

/* Funcion que cambia de base decimal a una base en funcion del alfabeto pasado por parametro y longitud de clave */
unsigned char *cambioBase(unsigned char alpha[], unsigned long long num, int key_len) {
    unsigned char *devolver = (unsigned char *) calloc(key_len, sizeof(unsigned char));
    // Before doing anything else, we fill up the entire "string" with the very first value of the alphabet, element [0]:
    memset(devolver, alpha[0], key_len);
    int base = strlen(alpha);
    int i = key_len - 1;
    unsigned long long cociente = num;
    // Ahora realizamos el cambio de base propiamente dicho:
    do {
        devolver[i] = alpha[cociente % base];
        cociente = cociente / base;
        i--;
    } while (cociente != 0);
    devolver[key_len] = '\0';
    return devolver;
}

/* Este es el programa principal. Recibimos los cuatro parametros antes descritos */
// TODO: recepcion de los parametros de entrada implementando "getopt" + eliminar codigo "hardcodeado"
int main(int argc, const char *argv[]) {
    /* Other alphabets for testing purposes: */
    //unsigned char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    //unsigned char alphabet[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    //unsigned char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    //TODO BY PARAMETER
    unsigned char alphabet[] = "0123456789ABCDEF";
    //TODO BY PARAMETER
    unsigned char *ejemplo_diggest = "F6E0A1E2AC41945A9AA7FF8A8AAA0CEBC12A3BCC981A929AD5CF810A090E11AE"; // 111 TODO: esto debiera ser pasado por parametro. Es SOLO un EJEMPLO.

    int lenAlpha = strlen(alphabet);
    //TODO BY PARAMETER
    int lenKeyMin = 2; // TODO: Por parametro tenemos que evitar que nos metan un 0, el minimo tendria que ser al menos siempre 1. Tenerlo en cuenta a la hora de implementar el 'getopt'
    //TODO BY PARAMETER
    int lenKeyMax = 5; // TODO: Asegurarnos de que el usuarios siempre meta un MAX, nunca puede ir este campo vacio. Tenerlo en cuenta a la hora de implementar el 'getopt'
    unsigned long long keyspace;
    unsigned long long i = 0;
    int j;

    //Private elements
    unsigned char *candidato;
    unsigned char *candidate_diggest;
    unsigned char buffer [65];
    int l;
    int comparacion;
    int stop = 0;
    // Generacion de TODAS las claves CANDIDATAS para una clave de un determinado tamaño comprendido entre MIN y MAX:
    for (j = lenKeyMin; j <= lenKeyMax; j++) {
        keyspace = mypow(lenAlpha, j);
        #pragma omp parallel for private(candidato, candidate_diggest, l, buffer, comparacion, stop)
        for (i = 0; i < keyspace; i++) {
            candidato = cambioBase(alphabet, i, j);
            // Hasheamos el candidato con nuestra funcion Hash:
            candidate_diggest = sha256_hasher(candidato);
            l = 0;
            for(l = 0; l < 32; l++) {
                sprintf(&buffer[2*l], "%02X", candidate_diggest[l]);
            }
            if (strcmp(ejemplo_diggest, buffer) == 0) {
                printf("Key: %s Text: %s\n", candidato, buffer);
                comparacion = strcmp((unsigned char *) ejemplo_diggest, buffer);
            }else{
                //printf("Key: %s\n", candidato);
            }
            //printf("%d / %d \n", omp_get_thread_num()/omp_get_num_threads());

            free(candidato);
            free(candidate_diggest);
        }
    }
    return 0;
}

