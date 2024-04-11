
#include <iostream>
#include <string>
#include "BaseClass.h"

Base::Base(std::string name)
{
	std::cout << "Create: " << name << std::endl;
	this->name = name;
}
Base::Base()
{
	this->num1 = 11;
	this->num2 = 22;
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

