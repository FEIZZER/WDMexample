#pragma once

#define Max_File_Len	260


typedef enum _LowLevelError
{
	LOWLEVEL_SUCCESS, 
	Init_Load_Dll_Err,
	Init_Parse_Dll_Err,

} LowLevelError;

LowLevelError __declspec(dllimport) Inject_Init_Template();
LowLevelError __declspec(dllimport) Inject_Init();
LowLevelError __declspec(dllimport) Inject(HANDLE hProcess);