#include <stdio.h>

int main(int argc, char **argv)
{
	if (argc == 2) {
		printf("Hello world! Today is %s %s.\n", argv[0], argv[1]);
		return 0;
	} else {
		fprintf(stderr,  "Error!");
		return 22;
	}
}
