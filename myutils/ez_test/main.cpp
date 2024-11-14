#include <iostream>
#include <thread>
#include <chrono>
#include "ez_thread_pool/ez_thread_pool.hpp"


void add(int a)
{
	for (int i = 0; i < a; i++)
	{
		printf("working..., %d/%d\n", i, a);
	}
}


int main()
{
	ez_thread_pool tp;

	int x = 1;
	tp.post_work_task(add, 20);
	tp.post_work_task(add, 10);
	tp.post_work_task(add, 30);
	tp.post_work_task(add, 40);
	tp.post_work_task(add, 50);
	tp.post_work_task(add, 60);
	tp.post_work_task(add, 70);

	std::this_thread::sleep_for(std::chrono::seconds(10));

}