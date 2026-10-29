#include <stdio.h>
int main()
{
	int i = 1;
	unsigned int j = -1;

	if (i < j)
		printf("%d is bigger then %d\n", j, i);
}

/* 

Compile : gcc -Wall main.c -o main  // OK
Compile : gcc -Wextra main.c -o main // warning: comparison between signed and unsigned integer expressions
Compile : gcc -Wpedantic main.c -o main // OK

*/
