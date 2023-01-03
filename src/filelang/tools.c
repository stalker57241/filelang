#include <filelang/tools.h>
#include <stdlib.h>

bool fltStrCmp(FLcstr str1, FLcstr str2) {
    int l1 = strlen(str1);
    
    if (l1 == strlen(str2)) {
        for (int i = 0; i < l1; i++) {
            if (str1[i] != str2[i]) {
                return false;
            }
        }
        return true;
    }
    else return false;
}