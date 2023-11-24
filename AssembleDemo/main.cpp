
#include <iostream>

int g_Count = 100;

void MyPrint() {
	return;
}

int Add(int a, int& b)
{
	return a + b;
}

int& GetCount() {
	int num = 10;
	num--;
	return g_Count;
}

int main() 
{
	std::shared_ptr<int> shPtr = std::make_shared<int>(20);
}