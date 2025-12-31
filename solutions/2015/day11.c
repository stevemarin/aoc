
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_CHARS 8

_Bool increment_password(char *password, int num_chars) {
    _Bool done = false;
    for (int idx = num_chars - 1; idx >= 0; idx--) {
        if (done) {
            return true;
        }

        if (password[idx] == 'z') {
            password[idx] = 'a';
            done = increment_password(password, num_chars - 1);
        } else {
            password[idx]++;
            return true;
        }
    }
    assert(false);
}

_Bool has_straight(char password[NUM_CHARS], int num_chars) {
    for (int idx = 0; idx < num_chars - 2; idx++) {
        char ch0 = password[idx];
        char ch1 = password[idx + 1];
        char ch2 = password[idx + 2];
        if (ch0 == ch1 - 1 && ch0 == ch2 - 2) {
            return true;
        }
    }
    return false;
}

_Bool no_iol(char password[NUM_CHARS]) {
    for (int idx = 0; idx < NUM_CHARS; idx++) {
        switch (password[idx]) {
        case 'i':
        case 'o':
        case 'l':
            return false;
            break;
        default:
        }
    
    }
    return true;
}

_Bool double_double(char password[NUM_CHARS]) {
    char ch = password[0];
    int pair_start = 0;
    _Bool found_pair = false;
    for (int idx = 1; idx < NUM_CHARS - 1; idx++) {
        char next_ch = password[idx + 1];
        if (ch == next_ch && !found_pair) {
            found_pair = true;
            pair_start = idx;
        } else if (ch == next_ch && found_pair && idx - pair_start > 1) {
            return true;
        }
        ch = next_ch;
    }
    return false;
}

int main(int argc, char *argv[]) {
    assert(argc == 2);
    FILE *fh = fopen(argv[1], "r");
    if (!fh) {
        printf("cannot find file: %s\n", argv[1]);
        exit(1);
    }

    char password[NUM_CHARS] = {0};
    for (int idx = 0; idx < NUM_CHARS; idx++) {
        int c = fgetc(fh);
        assert ('a' <= c && c <= 'z');
        password[idx] = (char)c;
    }
    assert(fgetc(fh) == '\n');

    _Bool found_one_already = false;
    while (true) {
        increment_password(password, NUM_CHARS);

        if (has_straight(password, NUM_CHARS) && no_iol(password) && double_double(password)) {
            if (found_one_already) {
                printf("second password is: %s\n", password);
                break;
            } else {
                found_one_already = true;
                printf("first password is: %s\n", password);
                increment_password(password, NUM_CHARS);
            }
        }
    }

    return 0;
}