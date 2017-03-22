/* This is our own implementation of the power function. Since we are not interested in working with doubles and that we want to avoid casting values from one type to another, we have implemented our own power function */
unsigned long long int mypow(unsigned long long int base, int exponente) {
	int i;
	unsigned long long int resul = 1;
	for (i = 0; i < exponente; i++) {
		resul *= base;
	}
	return resul;
}
