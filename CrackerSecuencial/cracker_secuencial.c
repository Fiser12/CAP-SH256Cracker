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
    unsigned char alphabet[] = "0123456789ABCDEF";
    unsigned char *ejemplo_diggest = "296D71A7F66E75B751C597094536329DCF2CF484F83E475D91F7AEA1FF4C9738"; // BA TODO: esto debiera ser pasado por parametro. Es SOLO un EJEMPLO.

    int lenAlpha = strlen(alphabet);
    int lenKeyMin = 1; // TODO: Por parametro tenemos que evitar que nos metan un 0, el minimo tendria que ser al menos siempre 1. Tenerlo en cuenta a la hora de implementar el 'getopt'
    int lenKeyMax = 2; // TODO: Asegurarnos de que el usuarios siempre meta un MAX, nunca puede ir este campo vacio. Tenerlo en cuenta a la hora de implementar el 'getopt'
    unsigned long long keyspace;
    unsigned long long i = 0;
    int j;

    // Generacion de TODAS las claves CANDIDATAS para una clave de un determinado tamaño comprendido entre MIN y MAX:
    for (j = lenKeyMin; j <= lenKeyMax; j++) {
        keyspace = mypow(lenAlpha, j);
        for (i = 0; i < keyspace; i++) {
            unsigned char *candidato = cambioBase(alphabet, i, j);
            //printf("(i:%d) Clave candidata: %s", i, candidato); // 'i' corresponde a la clave candidata posicion 'i' del total de claves posibles.
            // Hasheamos el candidato con nuestra funcion Hash:
            unsigned char *candidate_diggest = sha256_hasher(candidato);
            int l = 0;
            unsigned char buffer [65];
            for(l = 0; l < 32; l++) {
                sprintf(&buffer[2*l], "%02X", candidate_diggest[l]);
            }
            if (strcmp(ejemplo_diggest, buffer) == 0) {
                printf("%s \t | \t %s\n", buffer, ejemplo_diggest);
                printf("\n");
                printf("%d | %d \n", strlen(ejemplo_diggest), strlen(buffer));
                int comparacion = strcmp((unsigned char *) ejemplo_diggest, candidate_diggest);
                printf("Comparacion: %d\n", comparacion);
            }
            free(candidato);
            free(candidate_diggest);
        }
    }

    // No borrar: pruebas para la funcion sha256!!
    /*
    unsigned char *digest;
    //unsigned char kaixo[] = "kaixo";
    unsigned char *kaixo = "A";
    digest = sha256_hasher(kaixo);
    int l = 0;
    for (; l < strlen(digest); l++) {
        printf("%02X", digest[l]);
    }
    printf("\n");
    free(digest);
    */

    return 0;
}
