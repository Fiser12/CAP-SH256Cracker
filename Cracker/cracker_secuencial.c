#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // para el 'getOpt'
#include "mypow.h" // Implementacion de nuestra propia version de 'pow'
#include "hasher/sha256.h" // Implementacion del hasher. En nuestro caso: SHA256

#define MIN 1
#define MAX 4
#define ALPHABET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
#define HASHLENGTH 64

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

/* Este es el programa principal. Recibimos los cuatro parametros antes descritos (parametros opcionales, el <hash> obligatorio!) */
int main(int argc, char *argv[]) {   

    unsigned char *alphabet = NULL; // Alfabeto que vamos a utilizar para crackear el <hash>
    unsigned char *ejemplo_diggest = "F6E0A1E2AC41945A9AA7FF8A8AAA0CEBC12A3BCC981A929AD5CF810A090E11AE"; // 111
    int lenKeyMin = -1; // Valores no validos iniciales antes del parseo de parametros
    int lenKeyMax = -1; // Valores no validos iniciales antes del parseo de parametros 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int c; // Caracter de opcion que parseamos (si existe...)
	opterr = 0; // If you set this variable to zero, getopt does not print any messages, but it still returns the character '?' to indicate an error

    // Parseamos los parametros de entrada:
    while ((c = getopt (argc, argv, "a:n:m:")) != -1)
    switch (c) {
      case 'a':
        alphabet = optarg; // TODO: como copiamos esto en nuestro cracker?
        break;
      case 'n':
        lenKeyMin = atoi(optarg);
        break;
      case 'm':
        lenKeyMax = atoi(optarg);
        break;
      case '?':
	// Si 'getOpt' no reconoce un caracter de opcion dado, guarda dicho caracter en 'optopt' y devuelve '?'
        // Si 'getOpt' encuentra un caracter de opcion reconocible, pero no se encuentra valor (missing) del parametro, entonces guarda dicho
	// caracter en 'optopt' y devuelve '?'.
        if (optopt == 'a' || optopt == 'n' || optopt == 'm')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
	break;
      default:
	fprintf (stderr, "A problem was detected. No option character written, nor unknown option.");
        exit(EXIT_FAILURE); // Salimos de la ejecucion del cracker. Algo raro ha debido de pasar.
    }

	// TODO: Hacer un uppercase del diggest introducido para poder machearlo bien
	// Miramos si queda algun parámetro sin caracter de opcion (es decir, un parametro suelto sin precedente). En este caso, deberia ser el hash.
  if (optind + 1 == argc) { 
	// hay hash!
	ejemplo_diggest = argv[optind];
  } else {
    // Si no hay más parametros o hay más de uno, mostramos por pantalla una alerta al usuario.
    //diggest[0] = '\0'; // TODO: Guardamos caracter NULO????
    printf("Error, usage: %s [-a \"ALPHABET\"] [-n MIN] [-m MAX] <hash>\n", argv[0]);
  }

	// Si el usuario no ha introducido los parametros opcionales y solo el hash como tal. Inicializamos a valores por defecto (DEFINES):
	if (lenKeyMin == -1)
		lenKeyMin = MIN;
	if (lenKeyMax == -1)
	   	lenKeyMax = MAX;
	if (!alphabet) {
	    	alphabet = ALPHABET;
	}

	printf("---------------\n");
	printf("· alfabeto: %s\n", alphabet);
	printf("· min: %d\n", lenKeyMin);
	printf("· max: %d\n", lenKeyMax);
	printf("· diggest: %s\n", ejemplo_diggest);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    exit(EXIT_FAILURE); // barrera para ver si funciona todo correctamente...
    int lenAlpha = strlen(alphabet);
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
    for (j = lenKeyMin; j <= lenKeyMax&&!stop; j++) {
        keyspace = mypow(lenAlpha, j);
        for (i = 0; i < keyspace&&!stop; i++) {
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
                stop=1;
            }else{
                //printf("Key: %s\n", candidato);
            }
            free(candidato);
            free(candidate_diggest);
        }
    }
    return 0;
}
