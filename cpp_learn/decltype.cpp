

int add(int a, int b)
{
	return a + b;
}
void decltype_test()
{
	decltype((add(1, 2))) c = 1;
	decltype(c) d = 2;
	decltype(1 + 2) e = 3;
	// decltype( c =  1 + 2 ) e = 4;
	decltype(c = 1 + 2) f = e;
}