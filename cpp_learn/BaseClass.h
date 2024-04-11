#ifndef BaseClass
#define BaseClass
#include <iostream>
#include <string>


class Base
{
public:
	Base(std::string name);
	Base();
	~Base();
	void SayMyName();
	void SetMyName(std::string name);

	static Base GetInstance()
	{
		static Base base("default");
		return base;
	}



private:
	std::string name;
	int num1;
	int num2;
};



#endif // !BaseClass