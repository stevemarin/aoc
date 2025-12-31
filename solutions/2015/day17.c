
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    assert (argc == 2);
    FILE *fh = fopen(argv[1], "r");
    if (fh == nullptr) {
        printf("cannot open file: %s", argv[1]);
        exit(1);
    }

    return 0;
}