#include <stdio.h>

int main()
{
	int a[0];
	return 0;
}

/* 

	Compile : gcc -Wall main.c -o main  // unused variable 'a'
	Compile : gcc -Wextra main.c -o main // OK
	Compile : gcc -Wpedantic main.c -o main // ISO C forbids zero-size array 'a'

*/
