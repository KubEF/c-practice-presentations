#include <stdio.h>
#include "distance.h"

int main(int argc, char **argv) 
{
	int a,b;

	sscanf(argv[1], "%d", &a);
	sscanf(argv[2], "%d", &b);

	int res = distance(a, b);
	printf("%d\n", res);

	return 0;
}
