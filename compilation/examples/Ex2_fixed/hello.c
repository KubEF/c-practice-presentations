#include <stdio.h>
#include <errno.h>

int main(int argc, char **argv)
{
	if (argc == 3) {
		int date;
		char month[10];

		if (sscanf(argv[1], "%s", month) != 1) {
			fprintf(stderr,  "Error: Invalid input format!\n");
			return -EINVAL;
		}

		if (sscanf(argv[2], "%d", &date) != 1) {
			fprintf(stderr,  "Error: Invalid input format!\n");
			return -EINVAL;
		}

		if (date < 1 || date > 31) {
			fprintf(stderr,  "Error: Date must be between 1 and 31!\n");
			return -EINVAL;
		}

		printf("Hello world! Today is %s %d.\n", month, date);
		return 0;
	} else {
		fprintf(stderr,  "Error!\n");
		return -EINVAL;
	}
}
