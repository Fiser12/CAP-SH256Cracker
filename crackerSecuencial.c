//
// Created by fiser on 10/03/17.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

unsigned char* fromDeci(unsigned char alpha[], int base, int inputNum, int length)
{
    unsigned char * devolver = (unsigned char *)calloc(length, sizeof(unsigned char));
    int num = inputNum;
    int i = length - 1;
    while(i!=0){
        int index = num%(base);
        devolver[i] = alpha[index];
        num = num/base;
        i--;
    }
    devolver[i] = alpha[num];

    return devolver;
}
int main(int argc, const char* argv[]) {
    unsigned char alpha[] = "0123456789ABCDEF";
    int lenKeyMax = 3;
    int lenKeyMin = 2;
    int lenAlpha = strlen(alpha);
    int i = 0;
    for(int j = lenKeyMin; j<=lenKeyMax; j++) {
        int valor = pow(lenAlpha, j);
        for(i = 0; i<valor; i++){
            unsigned char *result = fromDeci(alpha, lenAlpha, i, j);
            printf("%d de %d es %s\n", valor, i, result);
            free(result);
        }
    }
    return 0;
}
