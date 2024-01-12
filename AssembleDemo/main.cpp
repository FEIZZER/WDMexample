
#include <iostream>
#include <string>
//int g_Count = 100;
//
//void MyPrint() {
//	return;
//}
//
//int Add(int a, int& b)
//{
//	return a + b;
//}
//
//int& GetCount() {
//	int num = 10;
//	num--;
//	return g_Count;
//}

int main() 
{
	std::string str = "hello world";
	int idx = str.rfind("nonono");
	printf("idx: %d", idx);
}