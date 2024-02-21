#ifndef BaseClass
#define BaseClass
#include <iostream>
#include <string>


class Base
{
public:
	Base(std::string name);
	~Base();
	void SayMyName();
	void SetMyName(std::string name);

private:
	std::string name;

};

Base::Base(std::string name)
{
	std::cout << "Create: " << name << std::endl;
	this->name = name;
}

Base::~Base()
{
	std::cout << "Destory: " << name << std::endl;
}

void Base::SayMyName()
{
	std::cout << "MyName: " << name << std::endl;
}

void Base::SetMyName(std::string name)
{
	this->name = name;
}

#endif // !BaseClass