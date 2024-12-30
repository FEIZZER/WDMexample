#pragma once
#include <string>
#include <atomic>
#include <thread>
#include <iostream>

class ez_msg_queue
{
public:
	using work_function = void(*)(void*, unsigned int);

	ez_msg_queue(const std::string& mail_slot_name, work_function work_function);
	ez_msg_queue(const std::string& mail_slot_name, work_function work_function, unsigned int interval_time);
	ez_msg_queue(const std::string& mail_slot_name, work_function work_function, unsigned int interval_time, unsigned int max_buffer_length);
	~ez_msg_queue();

	bool push_msg_2_this(const void* const buffer, const unsigned int& buffer_length);

	static bool push_msg(const std::string& mail_slot_name, int& err_code);
	static bool push_msg(uintptr_t handle, int& err_code);

	std::string get_mail_slot_name();

private:
	void work_loop();

private:
	std::atomic_bool	exit_;
	std::thread			work_thread_;
	work_function		work_function_;

	std::string			mail_slot_name_;
	unsigned int		interval_time_;
	unsigned int		max_buffer_length_;
	uintptr_t			mail_slot_handle_;
};