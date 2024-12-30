#include "ez_msg_queue/ez_msg_queue.h"
#include "Windows.h"

ez_msg_queue::ez_msg_queue(const std::string& mail_slot_name, work_function work_function)
	: ez_msg_queue(mail_slot_name, work_function, 1000)
{}

ez_msg_queue::ez_msg_queue(const std::string& mail_slot_name, work_function work_function, unsigned int interval_time)
	: ez_msg_queue(mail_slot_name, work_function, interval_time, 10240)
{}

ez_msg_queue::ez_msg_queue(const std::string& mail_slot_name, work_function work_function, unsigned int interval_time, unsigned int max_buffer_length)
	: mail_slot_name_("\\\\.\\mailslot\\" + mail_slot_name), work_function_(work_function), interval_time_(interval_time), max_buffer_length_(max_buffer_length)
{
	auto hSlot = CreateMailslotA(mail_slot_name_.c_str(), 0, interval_time_, NULL);
	if (hSlot == NULL || hSlot == INVALID_HANDLE_VALUE)
	{
		std::string exception_msg = "CreateMailSlotA Failed GetLastError: " + std::to_string(GetLastError());
		throw std::exception(exception_msg.c_str());
	}

	mail_slot_handle_ = (uintptr_t)hSlot;
	exit_.store(false);
	work_thread_ = std::thread(&ez_msg_queue::work_loop, this);
}

ez_msg_queue::~ez_msg_queue()
{
	exit_.store(true);
	if (work_thread_.joinable())
	{
		work_thread_.join();
	}

	if (mail_slot_handle_ != 0)
	{
		CloseHandle((HANDLE)mail_slot_handle_);
	}

	mail_slot_handle_ = 0;
	interval_time_ = 0;
}

bool ez_msg_queue::push_msg_2_this(const void* const buffer, const unsigned int& buffer_length)
{
	auto hSlot = CreateFileA(mail_slot_name_.c_str(), GENERIC_WRITE, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hSlot == INVALID_HANDLE_VALUE || hSlot == NULL)
	{
		return false;
	}

	unsigned int write_buffer_length;
	if (!WriteFile(hSlot, buffer, buffer_length, (DWORD*)&write_buffer_length, NULL))
	{
		std::cout << "write failed: " << GetLastError() << std::endl;
		return false;
	}
	std::cout << "push msg success" << std::endl;
	return true;
}



bool ez_msg_queue::push_msg(uintptr_t handle, int& err_code)
{

}

std::string ez_msg_queue::get_mail_slot_name()
{
	return mail_slot_name_;
}





void ez_msg_queue::work_loop()
{
	DWORD nLastError;
	unsigned int read_buffer_length = 0;
	void* buffer = malloc(max_buffer_length_);
	if (buffer == nullptr)
	{
		// log
		return;
	}

	try{
		while (!exit_.load())
		{
			memset(buffer, 0, max_buffer_length_);
			read_buffer_length = 0;
			if (ReadFile((HANDLE)mail_slot_handle_, buffer, max_buffer_length_, (DWORD*)&read_buffer_length, NULL))
			{
				work_function_(buffer, read_buffer_length);
				continue;
			}

			nLastError = GetLastError();
			if (nLastError == ERROR_SEM_TIMEOUT)
			{

				continue;
			}

			break;
		}
	}
	catch(std::exception& e)
	{
		std::cout << "catch e: " << e.what() << std:: endl;
	}
	catch (...)
	{
		std::cout << "unknow error" << std::endl;
	}
	std::cout << "thread exit" << std::endl;

	free(buffer);
}