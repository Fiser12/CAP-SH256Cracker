//
// Created by fiser on 10/03/17.
// Parametros (hash, alfaveto, mínimo de clave, máximo de clave)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

unsigned char* fromDeci(char alpha[], int base, int inputNum, int length)
{
    unsigned char * devolver = (unsigned char *)calloc(length-1, sizeof(unsigned char));
    int num = inputNum;
    int i = 0;
    do{
        int index = num%(base+1);
        devolver[i] = alpha[index];
        num = num/base;
        i++;
    }while(num>=base);
    devolver[i] = alpha[num%(base+1)];

    return devolver;
}
int main(int argc, const char* argv[]) {
    unsigned char alpha[] = "01231456789ABCDEF";
    int lenKey = 2;
    int lenAlpha = strlen(alpha)-1;
    int i = 0;
    int valor = pow(lenAlpha, lenKey);
    for(i = 0; i<valor; i++){
        unsigned char * result = fromDeci(alpha, lenAlpha, i, lenKey);
        printf("%d de %d es %s\n", valor, i, result);
    }
    return 0;
}
