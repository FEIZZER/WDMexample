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


int main()
{
	// template class ez_thread_pool<int, void(*)(int&)>;
	ez_thread_pool<WorkMsg, decltype(&work)> tp(work);


	for (int i = 0; i < 20; i++)
	{
		tp.post_work_item({ 10 + i });
	}

	return 0;
}