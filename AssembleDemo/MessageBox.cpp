#include "windows.h"
#include <iostream>
#include <cstring>

int main() 
{
	std::cout<<"pid:"<< GetCurrentProcessId()<< std::endl;
	std::string str;
	do
	{
		std::cin >> str;
		if (str == "quit") 
		{
			break;
		}
		MessageBoxW(NULL, L"my name is feizzer", L"This is a Title", MB_OK);
	} while (true);

	return 0;
}