#pragma once

#define Max_File_Len	260


typedef enum _LowLevelError
{
	LOWLEVEL_SUCCESS, 
	Init_Load_Dll_Err,
	Init_Parse_Dll_Err,

} LowLevelError;

LowLevelError Inject_Init_Template();
LowLevelError Inject_Init();
LowLevelError Inject(HANDLE hProcess);