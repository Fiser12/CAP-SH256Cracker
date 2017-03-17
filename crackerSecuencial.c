//
// Created by fiser on 10/03/17.
// Parametros (hash, alfaveto, mínimo de clave, máximo de clave)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
unsigned char* fromDeci(char alpha[], int base, int inputNum, int length)
{
    unsigned char * devolver = (unsigned char *)calloc(length, sizeof(unsigned char));
    int i = 0;
    do{
        devolver[i] = alpha[inputNum%base];
        inputNum = inputNum/base;
        i++;
    }while(inputNum>=base);
    devolver[i] = inputNum;

    return devolver;
}
int main(int argc, const char* argv[]) {
    unsigned char alpha[] = "01231456789ABCDEF";
    int lenKey = 3;
    int lenAlpha = strlen(alpha);
    int i = 0;
    for(i = 0; i<lenAlpha^lenKey; i++){
        printf("%s\n", fromDeci(alpha, lenAlpha, i, lenAlpha*lenKey));
    }
    return 0;
}
