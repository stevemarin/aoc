
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define CHARS_PER_LINE 16

_Bool is_vowel(int ch) {
    switch (ch) {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
        return true;
    default:
        return false;
    }
}

int part1(FILE *fh) {
    int prev_ch = fgetc(fh), ch = 0;

    uint8_t num_vowels = 0;
    _Bool has_double_letter = false, saw_bad_letters = true;

    if (is_vowel(prev_ch)) {
        num_vowels++;
    }

    int num_nice = 0;
    while ((ch = fgetc(fh)) != EOF) {
        if ((prev_ch == 'a' && ch == 'b') || (prev_ch == 'c' && ch == 'd') || (prev_ch == 'p' && ch == 'q') ||
            (prev_ch == 'x' && ch == 'y')) {
            saw_bad_letters = true;
        }

        if (is_vowel(prev_ch)) {
            num_vowels++;
        }

        if (prev_ch == ch) {
            has_double_letter = true;
        }

        if (ch == '\n') {
            if (has_double_letter && !saw_bad_letters && num_vowels > 2) {
                num_nice++;
            }
            has_double_letter = false;
            saw_bad_letters = false;
            num_vowels = 0;
        }

        prev_ch = ch;
    }

    return num_nice;
}

int part2(FILE *fh) {
    rewind(fh);

    int num_nice = 0;
    int line[16] = {0};
    while (fgetc(fh) != EOF) {
        fseek(fh, -1, SEEK_CUR);

        for (int idx = 0; idx < CHARS_PER_LINE; idx++) {
            line[idx] = fgetc(fh);
        }

        // get dangling newline
        assert(fgetc(fh) == '\n');

        _Bool found_repeat = false, found_sandwich = false;
        for (int idx = 0; idx < CHARS_PER_LINE; idx++) {
            if (!found_sandwich && idx + 2 < CHARS_PER_LINE) {
                if (line[idx] == line[idx + 2]) {
                    found_sandwich = true;
                }
            }

            if (!found_repeat) {
                int ch = line[idx];
                int next_ch = line[idx + 1];

                for (int iii = idx + 2; iii < CHARS_PER_LINE; iii++) {
                    if (line[iii] == ch && line[iii + 1] == next_ch) {
                        found_repeat = true;
                    }
                }
            }

            if (found_repeat && found_sandwich) {
                num_nice++;
                break;
            }
        }
    }

    return num_nice;
}

int main(int argc, char *argv[]) {
    assert(argc == 2);
    FILE *fh = fopen(argv[1], "r");
    if (!fh) {
        printf("cannot find file: %s", argv[1]);
        exit(1);
    }

    int num_nice = part1(fh);
    printf("part 1 num nice: %i\n", num_nice);

    num_nice = part2(fh);
    printf("part 2 num nice: %i\n", num_nice);

    return 0;
}
