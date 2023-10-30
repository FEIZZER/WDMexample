//
// Created by DELL on 2023/9/4.
//
#include <stdbool.h>

bool LoadFile(char*, void**, unsigned long*);

bool IsPE(void*, unsigned long long);

int ClassifyPE(void*, unsigned long long);

bool ParseImportTable(void*, unsigned long long);

