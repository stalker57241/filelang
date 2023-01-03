#include <filelang/reader/reader.h>
#include <stdio.h>
#include <stdlib.h>
FLuint32_t flrReadFile(FLcstr path, FLcstr* pcode) {

    FILE* fptr;
    FLuint32_t exitcode = 0;
    if ((exitcode = fopen_s(&fptr, path, "r")) != 0) {
        return exitcode;
    }

    fseek(fptr, 0, SEEK_END);
    FLuint32_t fsize = ftell(fptr);
    *pcode = (FLcstr) calloc(fsize, sizeof(char));
    fseek(fptr, 0,  SEEK_SET);
    int i = 0;
    while (!feof(fptr)) {
        char buffer_c = fgetc(fptr);
        (*pcode)[i] = buffer_c;
        // printf("%c", buffer_c);
        i++;
    }

    fclose(fptr);

    return exitcode;
}