#pragma once
#ifndef PCH_H
#define PCH_H

// ���Ҫ�ڴ˴�Ԥ����ı�ͷ
#include "stdbool.h"

#define PNTHEADER(base)  ((PIMAGE_NT_HEADERS)(((PIMAGE_DOS_HEADER)base)->e_lfanew + (ULONG_PTR)base))


#endif //PCH_H
