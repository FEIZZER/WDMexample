#pragma once
#ifdef _WIN32
#include <memory>
#include <strsafe.h>
#include <debugapi.h>
namespace ez {

#define OUTPUT_MSG_MAXLEN		2024

void DebugStringA(const char* out_string, ...) {
	char msg[OUTPUT_MSG_MAXLEN] = { 0 };
	va_list al_args;

	va_start(al_args, out_string);
	StringCbVPrintfA(msg, OUTPUT_MSG_MAXLEN, out_string, al_args);
	va_end(al_args);

	OutputDebugStringA(msg);
}

void DebugStringW(const wchar_t* out_string, ...) {
	wchar_t msg[OUTPUT_MSG_MAXLEN] = { 0 };
	va_list al_args;

	va_start(al_args, out_string);
	StringCbVPrintfW(msg, OUTPUT_MSG_MAXLEN, out_string, al_args);
	va_end(al_args);

	OutputDebugStringW(msg);
}


#define ez_debug(...) DebugStringA(__VA_ARGS__)


}
#endif // WIN32

