
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    assert(argc == 2);

    const char *filename = argv[1];

    FILE *fh = fopen(filename, "r");
    if (!fh) {
        printf("cannot find file: %s", filename);
        exit(1);
    }

    int floor = 0, first_basement = 0, counter = 0, ch;

    while ((ch = fgetc(fh)) != EOF) {
        counter++;
        switch (ch) {
        case '(':
            floor++;
            break;
        case ')':
            floor--;
            if (floor == -1 && first_basement == 0) {
                first_basement = counter;
            }
            break;
        default:
        }
    }

    printf("final floor = %i\n", floor);
    printf("first basement = %i\n", first_basement);

    return 0;
}
