/*********************************************************************
* Filename:   sha256.c
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Performs known-answer tests on the corresponding SHA1
	          implementation. These tests do not encompass the full
	          range of available test vectors, however, if the tests
	          pass it is very, very likely that the code is correct
	          and was compiled properly. This code also serves as
	          example usage of the functions.
*********************************************************************/

/*************************** HEADER FILES ***************************/
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "sha256.h"

/*********************** FUNCTION DEFINITIONS ***********************/
void sha256_hasher(BYTE text1[])
{
	//BYTE text1[] = {"abc"};
	/*BYTE hash1[SHA256_BLOCK_SIZE] = {0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,
	                                 0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad};*/
	BYTE buf[SHA256_BLOCK_SIZE];
	SHA256_CTX ctx;
	int pass = 1;

	sha256_init(&ctx);
	sha256_update(&ctx, text1, strlen(text1));
	sha256_final(&ctx, buf);
	printf("Hash outputed: %x\n", buf);
}

int main(int argc, char* argv[])
{	
	printf("Programe name: %s\n", argv[0]);
	printf("Introduced parameter: %s\n", argv[1]);
	(argv[1] != NULL ) ? sha256_hasher(argv[1]) : printf("No input text!! :(\n");

	return(0);
}
