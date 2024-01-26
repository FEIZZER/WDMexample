#include <stdio.h>

#include "windows.h"
#include "../low_level_inject/low_level_inject.h"

int main() 
{
	printf("init start!");

	Inject_Init_Template();

	printf("init done!");

	return 0;
}