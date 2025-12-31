
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int part1(FILE *fh) {
    int diff = 0;

    int ch, next_ch;
    while ((ch = fgetc(fh)) != EOF) {
        switch (ch) {
        case '\\':
            switch (next_ch = fgetc(fh)) {
            case 'x':
                diff += 3;
                break;
            default:
                diff += 1;
                break;
            }
            break;
        case '"':
            diff += 1;
            break;
        default:
        }

    }

    return diff;
}

int part2(FILE *fh) {
    rewind(fh);

    int diff = 0;

    int ch;
    while ((ch = fgetc(fh)) != EOF) {
        switch (ch) {
        case '\n':
            diff += 2;
            break;
        case '\\':
            diff += 1;
            break;
        case '"':
            diff += 1;
            break;
        default:
        }
    }

    return diff;
}

int main(int argc, char *argv[]) {
    assert(argc == 2);
    FILE *fh = fopen(argv[1], "r");
    if (!fh) {
        printf("cannot find file: %s", argv[1]);
        exit(1);
    }

    int res = part1(fh);
    printf("code - chars = %i\n", res);

    res = part2(fh);
    printf("chars - code = %i\n", res);

    return 0;
}
