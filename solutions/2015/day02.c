
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_WIDTH 512

int main(int argc, char *argv[]) {
    assert(argc == 2);

    FILE *fh = fopen(argv[1], "r");
    if (!fh) {
        printf("cannot find file: %s", argv[1]);
        exit(1);
    }

    int iii = 0, idx = 0, lwh[3] = {}, areas[3] = {}, paper = 0, ribbon = 0;
    int ch, buf[MAX_LINE_WIDTH] = {};
    while ((ch = fgetc(fh)) != EOF) {
        switch (ch) {
        case 'x':
        case '\n':
            lwh[idx] = atoi((char *)buf);
            idx++;
            memset(buf, 0, sizeof(buf));
            iii = 0;

            if (ch == 'x') {
                break;
            }

            areas[0] = lwh[0] * lwh[1];
            areas[1] = lwh[0] * lwh[2];
            areas[2] = lwh[1] * lwh[2];

            int surface_area = 2 * (areas[0] + areas[1] + areas[2]);

            int min_side_area = areas[0] < areas[1] ? areas[0] : areas[1];
            min_side_area = min_side_area < areas[2] ? min_side_area : areas[2];

            int max_side_length = lwh[0] > lwh[1] ? lwh[0] : lwh[1];
            max_side_length = max_side_length > lwh[2] ? max_side_length : lwh[2];

            int min_perim = 2 * (lwh[0] + lwh[1] + lwh[2] - max_side_length);
            int volume = lwh[0] * lwh[1] * lwh[2];

            paper = paper + surface_area + min_side_area;
            ribbon = ribbon + min_perim + volume;
            idx = 0;
            break;
        default:
            buf[iii] = ch;
            iii++;
            break;
        }
    }

    printf("paper: %i\n", paper);
    printf("ribbon: %i\n", ribbon);

    return 0;
}