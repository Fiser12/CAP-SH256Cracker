#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // para el 'getOpt'
#include "mypow.h" // Implementacion de nuestra propia version de 'pow'

#define MIN 1
#define MAX 4
#define ALPHABET "0123456789ABCDEF"
#define ASOMARCABEZA 4

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

/* Este es el programa principal. Recibimos los cuatro parametros antes descritos (3 de ellos opcionales, el <hash> obligatorio!) */
int main(int argc, char *argv[]) {

    MPI_Status status; // Muestra el estado de la conexion (paralelizacion con MPI)
    unsigned char *alphabet = NULL; // Alfabeto a utilizar para probar contra el <hash>
    unsigned char *ejemplo_diggest = NULL; // "F6E0A1E2AC41945A9AA7FF8A8AAA0CEBC12A3BCC981A929AD5CF810A090E11AE"; // 111
    int lenKeyMin = -1; // Valores no validos iniciales antes del parseo de parametros
    int lenKeyMax = -1; // Valores no validos iniciales antes del parseo de parametros 

    ///////// START GETOPT IMPLEMENTATION: //////////////////////////////////////////////
    int c; // Caracter de opcion que parseamos (si existen...)
    opterr = 0; // If you set this variable to zero, getopt does not print any messages, but it still returns the character '?' to indicate an error
    // Parseamos los parametros de entrada, busamos una '-a', '-n' y/o '-m':
    while ((c = getopt(argc, argv, "a:n:m:")) != -1)
        switch (c) {
            case 'a':
                alphabet = optarg;
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
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                break;
            default:
                fprintf(stderr, "A problem was detected. No option character written, nor unknown option.");
                exit(EXIT_FAILURE); // Salimos de la ejecucion del cracker. Algo raro ha debido de pasar.
        }

    // Miramos si queda algun parámetro sin caracter de opcion (es decir, un parametro suelto sin precedente). En este caso, deberia ser el hash.
    if (optind + 1 == argc) {
        // hay hash!
        ejemplo_diggest = argv[optind];
        // Lo convertimos a UPPER CASE por si acaso:
        int p = 0;
        while (ejemplo_diggest[p] != '\0') {
            ejemplo_diggest[p] = toupper(ejemplo_diggest[p]);
            p++;
        }
    } else {
        // Si no hay más parametros o hay más de uno, mostramos por pantalla una alerta al usuario.
        printf("Error, usage: %s [-a \"ALPHABET\"] [-n MIN] [-m MAX] <hash>\n", argv[0]);
        exit(EXIT_FAILURE); // no seguimos ejecutando
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

    ///////// END GETOPT IMPLEMENTATION: //////////////////////////////////////////////
    // A continuacion comenzamos con la impementacion del cracker propiamente dicho:

    int lenAlpha = strlen(alphabet);
    unsigned long long keyspace; 
    unsigned long long i = 0;
    int j;
    int l;
    int rank, size, chunk_size, indice_comienzo, asomar_cabeza; // 'Size' es el numero de procesos en paralelo (MPI) 

    //Private elements
    unsigned char *candidato; // Clave candidata generada a partir de un determinado alfabeto con longitud de clave dada.
    unsigned char *candidate_diggest; // Hash de la clave candidata
    unsigned char buffer[65]; // Buffer auxiliar que utilizamos para formatear correctamente el hash que obtenemos de la clave candidata
    int stop = 0; // "Booleano" que indica cuando parar de buscar.
    int stopFinal = 0; // Sumatorio de los "booleanos" recibidos por cada Slave para que el master decida si parar o no.
    // Paralelizando con MPI:
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Generacion de TODAS las claves CANDIDATAS para una clave de un determinado tamaño comprendido entre MIN y MAX.
    // Hasheamos la clave candidata generada y la comparamos con el diggest introducido por el usuario por parametro.
    // Si coinciden: entonces encontrado! hemos crackeado la contrasena. SI NO, entonces nos vamos a ver una peli con palomitas.

    for (j = lenKeyMin; j <= lenKeyMax && !stopFinal; j++) {
        keyspace = mypow(lenAlpha, j);


        chunk_size = keyspace / size; // Cantidad de numeros que cada proceso ha de procesar, siendo 'Size' numero de procesos.
	asomar_cabeza = chunk_size / ASOMARCABEZA; // Cada proceso se va a poner a escuchar si alguno de los otros procesos ha acabado. Esto 						// se va a hacer cuatro veces durante los calculos que cada uno de ellos hace. El master orquesta
					// el resultado final y decide si parar o no.

        int k;

        if (rank == 0) {
            for (k = 1; k < size; k++) {
                int numero = chunk_size * k;
                //printf("NUMERO ENVIADO! (master)\n");
                MPI_Send(&numero, 1, MPI_UNSIGNED_LONG, k, 0, MPI_COMM_WORLD);
                indice_comienzo = 0;
            }
        } else {
            // Para cualquier otro proceso (incluido el master) recogemos el indice de inicio (por el cual parte cada proceso) y continuamos normal:
            MPI_Recv(&indice_comienzo, 1, MPI_UNSIGNED_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            //printf("NUMERO RECIBIDO! (proceso#%d) | numero = %d\n", rank, indice_comienzo);
        }
        // printf("%d | %d | %llu\n", indice_comienzo, chunk_size, i);

        for (i = indice_comienzo; i < (indice_comienzo + chunk_size)&&!stopFinal; i++) {
            //printf("Soy proceso#%d, me ocupo del numero:%llu\n", rank, i);
            // Generamos candidato:
            candidato = cambioBase(&alphabet[0], i, j);
            // Hasheamos el candidato con nuestra funcion Hash:
            candidate_diggest = sha256_hasher(candidato);
            // Formateamos adecuadamente el hash del candidato obtenido para poder compararlo con el <hash> introducido por el usuario:
            for (l = 0; l < 32; l++) {
                sprintf(&buffer[2 * l], "%02X", candidate_diggest[l]);
            }
            if (strcmp(ejemplo_diggest, buffer) == 0) {
                printf("Estas de ENHORABUENA! Match de diggests.\n· Diggest (hash): %s\n· Texto plano: %s\n",
                       buffer, candidato);
                stop = 1;
            }
            if(i == indice_comienzo + asomar_cabeza){
		printf("Asomar cabeza: %d (proceso#%d)\n", indice_comienzo + asomar_cabeza, rank);
		printf("Mi 'stop' (proceso#%d): %d\n", rank, stop);
                MPI_Allreduce(&stop, &stopFinal, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
                asomar_cabeza += asomar_cabeza;
		printf("STOP FINAL: %d (proceso#%d)\n", stopFinal, rank);
            }
        }

        // Ahora tratamos el resto:
        if (rank == 0 && keyspace % size != 0) {
            for (i = keyspace - (keyspace % size); i < keyspace&&!stopFinal; i++) {
                // Generamos candidato:
                candidato = cambioBase(&alphabet[0], i, j);

                // Hasheamos el candidato con nuestra funcion Hash:
                candidate_diggest = sha256_hasher(candidato);
                // Formateamos adecuadamente el hash del candidato obtenido para poder compararlo con el <hash> introducido por el usuario:
                for (l = 0; l < 32; l++) {
                    sprintf(&buffer[2 * l], "%02X", candidate_diggest[l]);
                }
                if (strcmp(ejemplo_diggest, buffer) == 0) {
                    printf("Estas de ENHORABUENA! Match de diggests.\n· Diggest (hash): %s\n· Texto plano: %s\n",
                           buffer, candidato);
                    stop = 1;
                }
                free(candidato); // evitamos que nuestra memoria RAM explote!
                free(candidate_diggest); // evitamos que nuestra memoria RAM explote!
            }
        }
    }

    MPI_Finalize();
    return 0;
}
