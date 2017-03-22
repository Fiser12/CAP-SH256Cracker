#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mypow.h"

/* Programa que dado un ALFABETO, un MINIMO y un MAXIMO de longitud de clave y el DIGGEST (generado por un hasher), genera una lista con tadas las claves candidatas posibles. */
/* ------------------------------------------------------ */

/* Funcion que cambia de base un determinado numero en base al alfabeto pasado por parametro */
unsigned char *cambioBase(unsigned char alpha[], unsigned long long num, int key_len)
{
    //unsigned char *devolver = (unsigned char *)calloc(key_len-1, sizeof(unsigned char));
    unsigned char devolver[key_len-1];
    int base = strlen(alpha);
    int i = key_len-1;
    unsigned long long cociente = num;
//printf("Base: %d\n", base);
//printf("i: %d\n", i);
//printf("cociente: %llu\n", cociente);
	
    do{
        devolver[i] = alpha[cociente%base]; printf("\t\tdevolver[%d] = %c\n", i, devolver[i]);
        cociente = cociente/base; //printf("\t\tCociente: %llu\n", cociente);
        i--; //printf("\t\ti: %d\n", i);
    }while(cociente != 0); 
    //devolver[i] = alpha[num]; ?????????? 
    printf("\t\tdevolver = %s\n", devolver);
    return devolver;
}

/* Este es el programa principal. Recibimos los cuatro parametros antes descritos */
// TODO: recepcion de los parametros de entrada implementando "getopt" + eliminar codigo "hardcodeado"
int main(int argc, const char* argv[]) {
	/* For testing purposes: */
	//unsigned char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	//unsigned char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    	unsigned char alphabet[] = "0123456789ABCDEF";
    	int lenKey = 4;
    	int lenAlpha = strlen(alphabet);
	unsigned long long keyspace = mypow(lenAlpha, lenKey); // Aqui calculamos el tamaño de nuestro keyspace. Es decir el total de combinaciones totales dado un alfabeto y la longitud de una hipotetica clave.
    	unsigned long long i;
    
	// Generacion de TODAS las claves CANDIDATAS para una clave de un determinado tamaño:
    	for(i = 0; i<4098; i++){
       		//printf("%d de %d es %s\n", keyspace, i, cambioBase(alpha, lenAlpha, i, lenKey));
		printf("Clave candidata: %s\n", cambioBase(alphabet, i, lenKey)); // 'i' corresponde a la clave candidata en la posicion 'i' del total.
	}

    return 0;
}
