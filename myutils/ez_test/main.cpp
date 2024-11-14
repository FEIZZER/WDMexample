#include <iostream>
#include "ez_thread_pool/ez_thread_pool.h"

void test_func(int& a)
{
	return;
}


struct WorkMsg
{
	int time;
};


void work(WorkMsg& msg)
{
	for (int i = 0; i <= msg.time; i++)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		printf("working..., %d/%d\n", i, msg.time);
	
	}
	printf("work done");
}

bool test_task(int a, int b)
{
	return true;
}

#include "ez_thread_pool/thread_pool_task.hpp"
int main()
{
	// template class ez_thread_pool<int, void(*)(int&)>;
	ez_thread_pool<WorkMsg, decltype(&work)> tp(work);
	for (int i = 0; i < 20; i++)
	{
		tp.post_work_item({ 10 + i });
	}

	ez_thread_pool_task tp_t;
	tp_t.post_work_task(test_task, 1, 2);

	return 0;
}