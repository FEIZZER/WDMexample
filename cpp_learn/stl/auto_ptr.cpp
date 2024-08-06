#ifndef auto_ptr_test
#define auto_ptr_test


#include "../BaseClass.h"
#include "auto_ptr.h"
void test(int* p)
{
	std::shared_ptr<int> ptr(p);
	std::cout << "used count:" << ptr.use_count() << std::endl;
}
void test2(std::shared_ptr<int> shared_ptr)
{
	std::cout << "test2 used count:" << shared_ptr.use_count() << std::endl;
}
void unique_ptr_test()
{
	// raw_ptr
	Base* base_ptr = new Base("feizzer");

	//shared_ptr
	auto shared_ptr = std::make_shared<int>(5);
	std::cout << "used count:" << shared_ptr.use_count() << std::endl;

	std::shared_ptr<int> shared_ptr2(shared_ptr);
	std::cout << "used count:" << shared_ptr.use_count() << shared_ptr2.use_count() << std::endl;

	test2(shared_ptr2);
	std::cout << "used count:" << shared_ptr.use_count() << shared_ptr2.use_count() << std::endl;

	test(shared_ptr.get());
	std::cout << "test done" << std::endl;
	std::cout << "is null:" << (shared_ptr == nullptr) << std::endl;
	std::cout << "is null:" << (shared_ptr.get() == nullptr) << std::endl;
	std::cout << "used count:" << shared_ptr.use_count() << std::endl;
	std::cout << "value:" << *shared_ptr << std::endl;

	//unique_ptr
	std::unique_ptr<Base> unique_base_ptr{ base_ptr };
	base_ptr->SayMyName();
	base_ptr->SetMyName("yougin");
	unique_base_ptr->SayMyName();

	delete base_ptr; 
	// unique_base_ptr->SayMyName();
}

#endif // !auto_ptr_test
