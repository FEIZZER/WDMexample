#pragma once
#include "stdbool.h"

int ClassifyPE32(char* buffer, unsigned long long size);


bool ParseExportTable32(char* base, unsigned long long size);

bool ParseImportTable32(char* base, unsigned long long size);