#include <stdio.h>

#include "windows.h"
#include "../low_level_inject/low_level_inject.h"

int main() 
{
	printf("init start!\n");

	if (Inject_Init_Template() != LOWLEVEL_SUCCESS)
	{
		printf("init failed\n");
		return 1;
	}
	else 
	{
		printf("init done!");
	}


	

	return 0;
}