#include <Windows.h>
#include <stdio.h>


int main()
{

	for (size_t i = 0; i < 9999; i++)
	{
		HANDLE hFile = CreateFileW(L"SharedLog.txt", GENERIC_READ, FILE_SHARE_READ, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			printf("exe: %d \n", i);
			WriteFile(hFile, "test", 4, NULL, NULL);
			CloseHandle(hFile);
		}
		else 
		{
			printf("err:%x\n", GetLastError());
		}
	}
	
}