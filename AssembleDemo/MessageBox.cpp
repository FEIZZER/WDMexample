#include "windows.h"
#include <iostream>
#include <cstring>

int main() 
{
	std::string str;
	do
	{
		std::cin >> str;
		if (str == "quit") 
		{
			break;
		}
		MessageBoxA(NULL, "my name is feizzer", "This is a Title", MB_OK);
	} while (true);

	return 0;
}