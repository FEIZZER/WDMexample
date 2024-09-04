
#include <iostream>
#include <string>
#include <Windows.h>

using hook_proc = uintptr_t(*)(ULONG targetProcessId);
using unhook_proc = bool(*)(uintptr_t proc_addr);

void Block()
{
	char in[1024];
	std::string str_quit = "quit";
	do {
		std::cout << "enter \" quit \" to quit" << std::endl;
		std::cin >> in;
		if (str_quit == in)
		{
			break;
		}
	} while (true);
}

int main(int args, char* argv[])
{

	if (args != 2)
	{
		std::cout << "wrong params" << std::endl;
		return 1;
	}

	auto nPid = std::stoi(argv[1]);

	auto hMod = LoadLibraryA("detours_hook_dll.dll");
	if (hMod == nullptr)
	{
		std::cout << "LoadLibraryA failed: " << GetLastError() << std::endl;
		return 1;
	}

	hook_proc proc = (hook_proc)GetProcAddress(hMod, "SetHook");
	if (proc == nullptr)
	{
		std::cout << "GetProcAddress failed: " << GetLastError() << std::endl;
		return 1;
	}
	uintptr_t hhook = proc(nPid);

	std::cout << "hhook:" << hhook << std::endl;
	

	 Block();
	
	unhook_proc unhook = (unhook_proc)GetProcAddress(hMod, "UnHook");
	if (unhook == nullptr)
	{
		std::cout << "GetProcAddress failed: " << GetLastError() << std::endl;
		return 1;
	}
	if (!unhook(hhook))
	{
		std::cout << "unhook failed:" << GetLastError() << std::endl;
	}

	// Block();

	return 0;
}