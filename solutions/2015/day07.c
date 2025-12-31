
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 8
#define MAX_LINE_LEN 64
#define CACHE_LENGTH 1024

typedef uint16_t (*Bitfunc)(uint16_t, uint16_t);
uint16_t and_func(uint16_t left, uint16_t right) { return left & right; }
uint16_t or_func(uint16_t left, uint16_t right) { return left | right; }
uint16_t lshift_func(uint16_t left, uint16_t right) { return left << right; }
uint16_t rshift_func(uint16_t left, uint16_t right) { return left >> right; }

void zero_buffer(char *buf, int len) {
    for (int iii = 0; iii < len; iii++) {
        buf[iii] = 0;
    }
}

void skip_through(FILE *fh, int ch) {
    while (fgetc(fh) != ch) {
        ;
    }
}
char peek(FILE *fh) {
    int ch = fgetc(fh);
    fseek(fh, -1, SEEK_CUR);
    assert (CHAR_MIN <= ch && ch <= CHAR_MAX);
    return (char)ch;
}

_Bool peek_digit(FILE *fh) {
    char ch = peek(fh);
    return '0' <= ch && ch <= '9';
}

_Bool peek_lower(FILE *fh) {
    char ch = peek(fh);
    return 'a' <= ch && ch <= 'z';
}

_Bool peek_upper(FILE *fh) {
    char ch = peek(fh);
    return 'A' <= ch && ch <= 'Z';
}

_Bool peek_letter(FILE *fh) {
    char ch = peek(fh);
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
}

_Bool is_digit(char ch) { return ('0' <= ch && ch <= '9'); }
_Bool is_lower(char ch) { return ('a' <= ch && ch <= 'z'); }
_Bool is_upper(char ch) { return ('A' <= ch && ch <= 'Z'); }
_Bool is_letter(char ch) { return is_lower(ch) || is_upper(ch); }

void skip_to_output_wire_name(FILE *fh) {
    int ch = fgetc(fh);
    while (true) {
        if (ch == EOF || ch == '>') {
            break;
        } else {
            ch = fgetc(fh);
        }
    }
    assert((ch = fgetc(fh)) == ' ');
}

uint16_t read_number(FILE *fh) {
    char buf[MAX_NAME_LEN] = {0};
    for (int idx = 0; idx <= MAX_NAME_LEN; idx++) {
        int ich = fgetc(fh);
        assert (CHAR_MIN <= ich && ich <= CHAR_MAX);
        char ch = (char)ich;
        if (is_digit(ch)) {
            buf[idx] = ch;
        } else {
            break;
        }
    }

    int val = atoi(buf);
    assert (0 <= val && val <= UINT16_MAX);
    return (uint16_t)val;
}

void read_name(FILE *fh, char buf[MAX_NAME_LEN]) {
    zero_buffer(buf, MAX_NAME_LEN);
    for (int idx = 0; idx <= MAX_NAME_LEN; idx++) {
        int ich = fgetc(fh);
        assert (CHAR_MIN <= ich && ich <= CHAR_MAX);
        char ch = (char)ich;
        if (is_lower(ch)) {
            buf[idx] = ch;
        } else {
            break;
        }
    }
}

void seek_to_start_of_wire_line(FILE *fh, char *wire_name) {
    rewind(fh);

    // move fh to start of line describing desired output wire
    assert(strlen(wire_name) <= 2);

    long offset = ftell(fh);
    char wire_name_buf[MAX_NAME_LEN] = {0};
    while (fgetc(fh) != EOF) {
        skip_to_output_wire_name(fh);
        read_name(fh, wire_name_buf);

        if (strcmp(wire_name, wire_name_buf) == 0) {
            fseek(fh, offset, SEEK_SET);
            break;
        } else {
            zero_buffer(wire_name_buf, MAX_NAME_LEN);
            offset = ftell(fh);
            continue;
        }
    }
}

int target_to_index(char target[MAX_NAME_LEN]) {
    for (int i = 2; i < MAX_NAME_LEN; i++) {
        assert(target[i] == 0);
    }

    int ones = (int)(target[0] - 'a') + 1;
    int tens = target[1] != 0 ? (int)(target[1] - 'a') + 1 : 0;

    return ones + 26 * tens;
}

uint16_t evaluate_wire(FILE *fh, char target[MAX_NAME_LEN], _Bool in_cache[CACHE_LENGTH],
                       uint16_t cache[CACHE_LENGTH]) {
    int target_idx = target_to_index(target);
    if (in_cache[target_idx]) {
        return cache[target_idx];
    }

    seek_to_start_of_wire_line(fh, target);

    // handle NOT if necessary
    _Bool found_not = false;
    if (peek(fh) == 'N') {
        found_not = true;
        skip_through(fh, ' ');
    }

    uint16_t left_value = 0;
    if (peek_digit(fh)) {
        left_value = read_number(fh);
    } else {
        char buf[MAX_NAME_LEN] = {0};
        read_name(fh, buf);
        fpos_t pos;
        assert(fgetpos(fh, &pos) == 0);
        left_value = evaluate_wire(fh, buf, in_cache, cache);
        assert(fsetpos(fh, &pos) == 0);
    }

    // single argument
    int ich = peek(fh);
    assert (CHAR_MIN <= ich && ich <= CHAR_MAX);
    char ch = (char)ich;
    if (ch == '-') {
        if (found_not) {
            uint16_t val = ~left_value;
            in_cache[target_idx] = true;
            cache[target_idx] = val;
            return val;
        } else {
            in_cache[target_idx] = true;
            cache[target_idx] = left_value;
            return left_value;
        }
    }

    Bitfunc op = NULL;
    assert((ich = fgetc(fh)) != EOF);
    assert (CHAR_MIN <= ich && ich <= CHAR_MAX);
    ch = (char)ich;

    switch (ch) {
    case 'L':
        op = (Bitfunc)lshift_func;
        break;
    case 'R':
        op = (Bitfunc)rshift_func;
        break;
    case 'A':
        op = (Bitfunc)and_func;
        break;
    case 'O':
        op = (Bitfunc)or_func;
        break;
    default:
        printf("unexpected character: %c", ch);
        exit(1);
        break;
    }

    skip_through(fh, ' ');

    uint16_t right_value = 0;
    if (peek_digit(fh)) {
        right_value = read_number(fh);
    } else {
        char buf[MAX_NAME_LEN] = {0};
        read_name(fh, buf);

        fpos_t pos;
        assert(fgetpos(fh, &pos) == 0);
        right_value = evaluate_wire(fh, buf, in_cache, cache);
        assert(fsetpos(fh, &pos) == 0);
    }

    assert(op != NULL);
    uint16_t val = (*op)(left_value, right_value);
    in_cache[target_idx] = true;
    cache[target_idx] = val;
    return val;
}

int part1(FILE *fh) {
    char target[MAX_LINE_LEN] = "a";
    _Bool in_cache[CACHE_LENGTH] = {0};
    uint16_t cache[CACHE_LENGTH] = {0};
    uint16_t res = evaluate_wire(fh, target, in_cache, cache);
    return res;
}

int part2(FILE *fh) {
    char target[MAX_LINE_LEN] = "a";
    _Bool in_cache[CACHE_LENGTH] = {0};
    uint16_t cache[CACHE_LENGTH] = {0};

    char b[MAX_NAME_LEN] = "b";
    int target_idx = target_to_index(b);
    in_cache[target_idx] = true;
    cache[target_idx] = 46065;

    uint16_t res = evaluate_wire(fh, target, in_cache, cache);
    return res;
}

int main(int argc, char *argv[]) {
    assert(argc == 2);
    FILE *fh = fopen(argv[1], "r");
    if (!fh) {
        printf("cannot find file: %s", argv[1]);
        exit(1);
    }

    int res = part1(fh);
    printf("part 1 wire a: %i\n", res);

    res = part2(fh);
    printf("part 2 wire a: %i\n", res);

    return 0;
}
