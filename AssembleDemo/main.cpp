
#include <iostream>
#include <stdio.h>

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
	int a = 20;
	int b = 23;
	int c = Add(a, b);

	GetCount() = 90;

	// printf("Count:%d", g_Count);

	return 0;
}