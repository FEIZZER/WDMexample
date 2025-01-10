#include <iostream>
#include <thread>
#include "ez_signal/signal.hpp"

ez::signal sig;
void wait_test(int i)
{
	std::cout << "start wait: " << i << std::endl;
	sig.wait();
	std::cout << "after wait: " << i << std::endl;
}

int main()
{
	auto th1 = std::thread(wait_test, 1);
	auto th2 = std::thread(wait_test, 2);
	auto th3 = std::thread(wait_test, 3);

	th1.detach();
	th2.detach();
	th3.detach();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	sig.notify_all();

	std::this_thread::sleep_for(std::chrono::seconds(5000));
}