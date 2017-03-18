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
        int index = inputNum%base;
        devolver[i] = alpha[index];
        inputNum = inputNum/base;
        i++;
    }while(i!=length);
    devolver[i] = inputNum;

    return devolver;
}
int main(int argc, const char* argv[]) {
    unsigned char alpha[] = "01231456789ABCDEF";
    int lenKey = 3;
    int lenAlpha = strlen(alpha)-1;
    int i = 0;
    for(i = 0; i<lenKey^(lenAlpha); i++){
        unsigned char * result = fromDeci(alpha, lenAlpha, i, lenKey);
        printf("%s\n", result);
    }
    return 0;
}
