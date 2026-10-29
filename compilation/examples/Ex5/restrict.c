void arr_copy(int* from, int* to, int how_much)
{
	for (int i = 0; i < how_much; ++i)
		from[i] = to[i];
}


void arr_copy_restrict(int* restrict from, int* restrict to, int how_much)
{
	for (int i = 0; i < how_much; ++i)
		from[i] = to[i];
}
