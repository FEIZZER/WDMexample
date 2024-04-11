
#include <iostream>
#include <Windows.h>



int main()
{
	HMODULE hDll = LoadLibraryA("C:\\Users\\DELL\\Desktop\\UhsFileDll.dll");
	if (hDll == nullptr)
	{
		std::cout << "LoadLib error:" << GetLastError() << std::endl;
	}
}