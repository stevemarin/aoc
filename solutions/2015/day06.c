
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CHARS_PER_LINE 64
#define GRID_SIZE 1000
#define BUF_SIZE 4

typedef _Bool (*f)(_Bool);
_Bool toggle_fn(_Bool val) { return !val; }
_Bool off_fn(_Bool val) { val = false; return val; }
_Bool on_fn(_Bool val) { val = true; return val; }

typedef int (*g)(int);
int toggle_fn2(int val) { return val + 2; }
int off_fn2(int val) { return val == 0 ? val : val - 1; }
int on_fn2(int val) { return val + 1; }

enum Op { TOGGLE, OFF, ON };

typedef struct IntInt {
    int val;
    int idx;
} IntInt;

IntInt get_number(char line[MAX_CHARS_PER_LINE], int idx, char ch) {
    char buf[BUF_SIZE] = {0};
    int iii = 0;
    for (; iii < BUF_SIZE; iii++) {
        if (line[idx + iii - 1] == ch) {
            break;
        }
        buf[iii] = line[idx + iii - 1];
    }

    IntInt ret = {.val = atoi(buf), .idx = idx + iii};
    return ret;
}

typedef struct Params {
    enum Op op;
    int start_row;
    int start_col;
    int end_row;
    int end_col;
} Params;

Params get_line_params(char line[MAX_CHARS_PER_LINE]) {
    int idx = 0;

    Params p;
    if (line[1] == 'o') {
        p.op = TOGGLE;
        idx = idx + 8; //sizeof("toggle ");
    } else if (line[6] == 'n') {
        p.op = ON;
        idx = idx + 9; //sizeof("turn on ");
    } else if (line[6] == 'f') {
        p.op = OFF;
        idx = idx + 10; //sizeof("turn off ");
    } else {
        printf("unprocessable line: %s\n", line);
    }

    IntInt ret = {.val = 0, .idx = idx};
    ret = get_number(line, ret.idx, ',');
    p.start_row = ret.val;
    ret = get_number(line, ret.idx + 1, ' ');
    p.start_col = ret.val;
    ret = get_number(line, ret.idx + 8, ','); // skip 'through '
    p.end_row = ret.val;
    ret = get_number(line, ret.idx + 1, 0);
    p.end_col = ret.val;

    return p;
}

void process_params_part1(Params p, _Bool mask[GRID_SIZE][GRID_SIZE]) {
    f funcs[3] = {&toggle_fn, &off_fn, &on_fn};

    // update values
    for (int row = p.start_row; row <= p.end_row; row++) {
        for (int col = p.start_col; col <= p.end_col; col++) {
            mask[row][col] = funcs[p.op](mask[row][col]);
        }
    }
}

int part1(FILE *fh) {
    _Bool mask[GRID_SIZE][GRID_SIZE] = {0};

    while (fgetc(fh) != EOF) {
        fseek(fh, -1, SEEK_CUR);

        char line[MAX_CHARS_PER_LINE] = {0};
        for (int idx = 0; idx < MAX_CHARS_PER_LINE; idx++) {
            int ch = fgetc(fh);
            if (ch == '\n') {
                Params p = get_line_params(line);
                process_params_part1(p, mask);
                break;
            } else {
                assert (0 <= ch && ch <= 255);
                line[idx] = (char)ch;
            }
        }
    }

    int num_on = 0;
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            num_on = mask[row][col] ? num_on + 1 : num_on;
        }
    }

    return num_on;
}

void process_params_part2(Params p, int mask[GRID_SIZE][GRID_SIZE]) {
    g funcs[3] = {&toggle_fn2, &off_fn2, &on_fn2};

    // update values
    for (int row = p.start_row; row <= p.end_row; row++) {
        for (int col = p.start_col; col <= p.end_col; col++) {
            mask[row][col] = funcs[p.op](mask[row][col]);
        }
    }
}

int part2(FILE *fh) {
    rewind(fh);

    int mask[GRID_SIZE][GRID_SIZE] = {0};

    while (fgetc(fh) != EOF) {
        fseek(fh, -1, SEEK_CUR);

        char line[MAX_CHARS_PER_LINE] = {0};
        for (int idx = 0; idx < MAX_CHARS_PER_LINE; idx++) {
            int ch = fgetc(fh);
            if (ch == '\n') {
                Params p = get_line_params(line);
                process_params_part2(p, mask);
                break;
            } else {
                assert (0 <= ch && ch <= 255);
                line[idx] = (char)ch;
            }
        }
    }

    int brightness = 0;
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            brightness = brightness + mask[row][col];
        }
    }

    return brightness;
}

int main(int argc, char *argv[]) {
    assert(argc == 2);
    FILE *fh = fopen(argv[1], "r");
    if (!fh) {
        printf("cannot find file: %s", argv[1]);
        exit(1);
    }

    int num_on = part1(fh);
    printf("num lights on: %i\n", num_on);
    assert (num_on == 543903);

    int brightness = part2(fh);
    printf("brightness: %i\n", brightness);
    assert (brightness == 14687245);

    return 0;
}
