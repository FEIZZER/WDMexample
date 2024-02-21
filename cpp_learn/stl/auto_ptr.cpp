#ifndef auto_ptr_test
#define auto_ptr_test


#include "../BaseClass.h"
#include "auto_ptr.h"
void unique_ptr_test()
{
	// raw_ptr
	Base* base_ptr = new Base("feizzer");

	//unique_ptr
	std::unique_ptr<Base> unique_base_ptr{ base_ptr };
	base_ptr->SayMyName();
	base_ptr->SetMyName("yougin");
	unique_base_ptr->SayMyName();

	delete base_ptr; 
	// unique_base_ptr->SayMyName();
}

#endif // !auto_ptr_test
