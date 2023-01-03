#include <filelang/main.h>


int main(int argc, char* argv[]) {
    printf("args:\n");
    for (int i = 0; i < argc; i++) {
        printf("%d: %s\n", i, argv[i]);
    }
    printf("\n");
    if (argc < 2) {
        printf("Not enough arguments");
        return 1;
    }
    char* code;

    flrReadFile(argv[argc - 1], &code);
    // printf("code:\n%s\n", code);
    FLtoken* token_arr;
    fltTokenize(code, &token_arr);
    free(token_arr);
    return 0;
}
