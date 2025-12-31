
#include "../../std/md5.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define KEY_LENGTH 16
#define BUF_LENGTH 512

int main(int argc, char *argv[]) {
    assert(argc == 2);
    FILE *fh = fopen(argv[1], "r");
    if (!fh) {
        printf("cannot find file: %s", argv[1]);
        exit(1);
    }

    int ch = 0, init[KEY_LENGTH] = {0};
    int counter = 0;
    while ((ch = fgetc(fh)) != EOF) {
        assert(counter < KEY_LENGTH);
        if ('a' <= ch && ch <= 'z') {
            init[counter] = ch;
            counter++;
        }
    }

    _Bool found_5 = false, found_6 = false;
    for (int num = 1;; num++) {
        char buf[BUF_LENGTH] = {0};
        sprintf(buf, "%s%i", (char *)init, num);

        uint8_t res[16] = {0};
        md5String(buf, res);

        if (found_5 == 0 && res[0] == 0 && res[1] == 0 && res[2] < 16) {
            printf("first with 5 zeros: %i\n", num);
            found_5 = true;
        }
        if (!found_6 && res[0] == 0 && res[1] == 0 && res[2] == 0) {
            printf("first with 6 zeros: %i\n", num);
            found_6 = true;
        }

        if (found_5 && found_6) {
            break;
        }
    }

    return 0;
}
