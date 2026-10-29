#include <stdio.h>

#define error(...) (fprintf(stderr, __VA_ARGS__))

#define abs1(x) x > 0 ? x : -x
#define abs2(x) ((x) > 0 ? (x) : (-x))

int main()
{
	int x = 7;
	int y = -8;

	int a1 = x + abs1(y);
	int a2 = x + abs2(y);

	if (a1 != a2)
		error("Error: %d != %d\n", a1, a2);

	return 0;
}
