#include <iostream>
#include <thread>
#include <chrono>
#include "ez_thread_pool/ez_thread_pool.hpp"

#include "windows.h"


void write_slot()
{
	auto hSlot = CreateFileA("\\\\.\\mailslot\\test_slot_name", GENERIC_WRITE, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSlot == NULL || hSlot == INVALID_HANDLE_VALUE)
	{
		std::cout << "create file failed: " << GetLastError() << std::endl;
		return;
	}

	for (int i = 0; i < 10; i++)
	{
		DWORD writedLen = 0;
		WriteFile(hSlot, (void*)"test_data", 10, &writedLen, NULL);
		std::cout << "write datalen:" << writedLen << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	std::cout << "write done" << std::endl;

}


void CreateMailSlot_test()
{
	auto hSlot = CreateMailslotA("\\\\.\\mailslot\\test_slot_name", 0, 1000, NULL);

	if (hSlot == NULL || hSlot == INVALID_HANDLE_VALUE)
	{
		return;
	}
	
	std::cout << "create slot success" << std::endl;
	
	auto start_wirte = std::thread(write_slot);

	start_wirte.detach();

	char pBuffer[1024] = { 0 };
	DWORD nBufLen = 0;
	while (true)
	{
		memset(pBuffer, 0, 1024);
		if (!ReadFile(hSlot, pBuffer, 1024, &nBufLen, NULL))
		{
			std::cout << "read file failed, err: " << GetLastError() << std::endl;
			continue;
		}
		std::cout << "read data: " << pBuffer << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

}

#include "ez_msg_queue/ez_msg_queue.h"

void test_work(void* buffer, unsigned int buffer_len)
{
	std::cout << (char*)buffer << std::endl;
}

int main()
{
	ez_msg_queue msg_queue("test", test_work);

	for (int i = 0; i < 10; i++)
	{
		msg_queue.push_msg_2_this((void*)"haha123", 8);
	}

	std::this_thread::sleep_for(std::chrono::seconds(5));

	std::cout << "Exit" << std::endl;
}