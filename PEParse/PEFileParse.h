//
// Created by DELL on 2023/9/4.
//
#include <stdbool.h>

bool LoadFile(char*, void**, unsigned long*);

bool IsPE(char*, unsigned long long);

int ClassifyPE(char*, unsigned long long);

bool ParseImportTable(char*, unsigned long long);

