#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mypow.h"
#include "hasher/sha256.h"

/* Programa que dado un ALFABETO, un MINIMO y un MAXIMO de longitud de clave y el DIGGEST (generado por un hasher), genera una lista con tadas las claves candidatas posibles. */
/* ------------------------------------------------------ */

/* Funcion que cambia de base un determinado numero en base al alfabeto pasado por parametro */
unsigned char * cambioBase(unsigned char alpha[], unsigned long long num, int key_len)
{
    	unsigned char *devolver = (unsigned char *)calloc(key_len+1, sizeof(unsigned char));
    	// Before doing anything else, we fill up the entire "string" with the very first value of the alphabet, element [0]:
	memset(devolver, alpha[0], key_len);
    	int base = strlen(alpha);
    	int i = key_len-1;
    	unsigned long long cociente = num;
	// Ahora realizamos el cambio de base propiamente dicho:
    	do {
        	devolver[i] = alpha[cociente%base];
        	cociente = cociente/base;
        	i--;
    	} while(cociente != 0);
    	return devolver;
}

/* Este es el programa principal. Recibimos los cuatro parametros antes descritos */
// TODO: recepcion de los parametros de entrada implementando "getopt" + eliminar codigo "hardcodeado"
int main(int argc, const char* argv[]) {
	/* Other alphabets for testing purposes: */
	//unsigned char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	//unsigned char alphabet[] = "abcdefghijklmnopqrstuvwxyz0123456789";
	//unsigned char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    	unsigned char alphabet[] = "0123456789ABCDEF";
	int lenAlpha = strlen(alphabet);
	int lenKeyMin = 1; // TODO: Por parametro tenemos que evitar que nos metan un 0, el minimo tendria que ser al menos siempre 1. Tenerlo en cuenta a la hora de implementar el 'getopt'
	int lenKeyMax = 4; // TODO: Asegurarnos de que el usuarios siempre meta un MAX, nunca puede ir este campo vacio. Tenerlo en cuenta a la hora de implementar el 'getopt'
	unsigned long long keyspace;
    	unsigned long long i = 0;
	int j;
    
	// Generacion de TODAS las claves CANDIDATAS para una clave de un determinado tamaño comprendido entre MIN y MAX:
	for(j = lenKeyMin; j<=lenKeyMax; j++) {
		keyspace = mypow(lenAlpha, j);
	    	for(i = 0; i<keyspace; i++){
	       		unsigned char *candidato = cambioBase(alphabet, i, j);
			printf("(i:%d) Clave candidata: %s\n", i, candidato); // 'i' corresponde a la clave candidata posicion 'i' del total de claves posibles.
			free(candidato);
		}
	}

	unsigned char *digest;
	unsigned char kaixo[] = "kaixo";
	digest = sha256_hasher(kaixo);
	// TODO: free ?????? de dentro de sha256_hasher
	int l = 0;
	printf("hola?\n");
	printf("%d\n", strlen(digest));
	for (; l < strlen(digest); l++) {
		printf("%02X", digest[l]);
	}
	printf("\n");
	

    return 0;
}
