//
// Created by DELL on 2023/9/4.
//
#include <stdio.h>
#include "PEFileParse.h"

int main() {
    char* buf = NULL;
    unsigned long size = 0;
    if (!LoadFile("E:\\messageBox32.exe", (void**)&buf, &size)) {
        printf("failed");
        return 1;
    }
    printf("%d",  ClassifyPE(buf, size));

    return 0;
}