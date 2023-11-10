//
// Created by DELL on 2023/9/4.
//

#include <stdio.h>
#include "PEFileParse.h"

int main() {
    char* buf = NULL;
    unsigned long size = 0;
    if (!LoadFile("E:\\DllDemo.dll", &buf, &size)) {
        printf("load file failed\n");
        return 1;
    }
    //printf("%d",  ClassifyPE(buf, size));
    ParseExportTable(buf, size);
    ParseImportTable(buf, size);
    return 0;
}