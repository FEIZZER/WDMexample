
#include <iostream>
#include <string>
#include <stdio.h>




int main2()
{
	long long number = 0x0102030405060708;
	printf("%llx\n", number);

	char* ch = (char*)&number;
	for (int i = 0; i < 8; i++)
	{
		printf("%x ", ch[i]);
	}
}