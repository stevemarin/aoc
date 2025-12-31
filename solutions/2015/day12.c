
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 16

int read_int(FILE *fh, int first) {
    char buf[BUF_SIZE] = {0};

    switch (first) {
    case '0' ... '9':
    case '-':
        buf[0] = (char)first;
        break;
    default:
        printf("invalid int->char -- %c\n", (char)first);
        exit(1);
    }

    for (int idx = 1; idx < BUF_SIZE; idx++) {
        int ch = fgetc(fh);
        switch (ch) {
        case '0' ... '9':
            buf[idx] = (char)ch;
            break;
        default:
            fseek(fh, -1, SEEK_CUR);
            return atoi(buf);
        }
    }

    assert(false);
}

int read_quoted(FILE *fh) {
    char buf[BUF_SIZE] = {'"'};

    for (int idx = 1; idx < BUF_SIZE; idx++) {
        int ch = 0;
        switch ((ch = fgetc(fh))) {
        case EOF:
            printf("found EOF while reading quoted chars\n");
            exit(1);
        case '"':
            buf[idx] = '"';
            if (idx == 4 && buf[1] == 'r' && buf[2] == 'e' && buf[3] == 'd') {
                return 0;
            } else {
                return 1;
            }
        default:
            assert('a' <= ch && ch <= 'z');
            buf[idx] = (char)ch;
            break;
        }
    }

    assert(false);
}

int part1(FILE *fh) {
    int sum = 0, ch = 0;
    while ((ch = fgetc(fh)) != EOF) {
        if (('0' <= ch && ch <= '9') || ch == '-') {
            sum += read_int(fh, ch);
        }
    }
    return sum;
}

int score_object(FILE *fh);
int score_list(FILE *fh);

int score_list(FILE *fh) {
    int ch = 0, score = 0;
    while ((ch = fgetc(fh)) != EOF) {
        switch (ch) {
        case ']':
            return score;
        case '0' ... '9':
        case '-':
            score += read_int(fh, ch);
            break;
        case '{':
            score += score_object(fh);
            break;
        case '[':
            score += score_list(fh);
            break;
        case '"':
            read_quoted(fh);
            break;
        case ',':
        case ' ':
            break;
        default:
            printf("unknown char in list: '%c'\n", ch);
            exit(1);
        }
    }
    return 0;
}

int score_object(FILE *fh) {
    int ch = 0, red = 1, score = 0;
    while ((ch = fgetc(fh)) != EOF) {
        switch (ch) {
        case '}':
            return red * score;
        case '{':
            score += score_object(fh);
            break;
        case '[':
            score += score_list(fh);
            break;
        case '"':
            red *= read_quoted(fh);
            break;
        case '0' ... '9':
        case '-':
            score += read_int(fh, ch);
            break;
        case ':':
        case ',':
        case ' ':
            break;
        default:
            printf("unknown char in object: '%c'\n", ch);
            exit(1);
        }
    }

    assert(false);
}

int part2(FILE *fh) {
    rewind(fh);

    int ch = 0, score = 0;
    while ((ch = fgetc(fh)) != EOF) {
        switch (ch) {
        case '[':
            score += score_list(fh);
            break;
        case '{':
            score += score_object(fh);
            break;
        case ',':
        case ' ':
        case '\n':
            break;
        default:
            printf("should be '[' or '{': got '%c'\n", ch);
            exit(1);
        }
    }

    return score;
}

int main(int argc, char *argv[]) {
    assert(argc == 2);
    FILE *fh = fopen(argv[1], "r");
    if (!fh) {
        printf("cannot find file: %s\n", argv[1]);
        exit(1);
    }

    int sum = part1(fh);
    printf("sum = %i\n", sum);
    assert(sum == 111754);

    sum = part2(fh);
    printf("sum = %i\n", sum);
    assert(sum == 65402);

    return 0;
}
