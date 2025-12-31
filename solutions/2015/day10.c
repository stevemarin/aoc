
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 5103800

int main(int argc, char *argv[]) {
    assert(argc == 2);
    FILE *fh = fopen(argv[1], "r");
    if (!fh) {
        printf("cannot find file: %s", argv[1]);
        exit(1);
    }

    char *current_buf = (char *)malloc(BUF_SIZE * sizeof(char));
    memset(current_buf, BUF_SIZE, sizeof(char));
    char *next_buf = (char *)malloc(BUF_SIZE * sizeof(char));
    memset(next_buf, BUF_SIZE, sizeof(char));

    for (int idx = 0; idx < BUF_SIZE; idx++) {
        int ch = fgetc(fh);
        if (ch == '\n') {
            break;
        } else {
            assert ('0' <= ch && ch <= '9');
            current_buf[idx] = (char)ch;
        }
    }

    fclose(fh);

    int buf_after_40 = 0;
    int buf_after_50 = 0;
    for (int iter = 0; iter < 50; iter++) {
        int counter = 1;
        int output_idx = 0;
        char current_ch = current_buf[0];
        for (int idx = 1; idx < BUF_SIZE; idx++) {
            char next_ch = current_buf[idx];
            if (current_ch == next_ch) {
                counter++;
            } else if (next_ch == 0) {
                break;
            } else {
                assert(1 <= counter && counter <= 9);

                next_buf[output_idx++] = (char)('0' + counter);
                next_buf[output_idx++] = current_ch;

                counter = 1;
                current_ch = next_ch;
            }
        }

        next_buf[output_idx++] = (char)('0' + counter);
        next_buf[output_idx++] = current_ch;

        for (int idx = 0; idx < BUF_SIZE; idx++) {
            current_buf[idx] = next_buf[idx];
            next_buf[idx] = 0;
        }

        if (iter == 39) {
            for (int idx = 0; idx < BUF_SIZE; idx++) {
                if (current_buf[idx] != 0) {
                    buf_after_40++;
                } else {
                    break;
                }
            }
        }
    }

    for (int idx = 0; idx < BUF_SIZE; idx++) {
        if (current_buf[idx] != 0) {
            buf_after_50++;
        } else {
            break;
        }
    }

    printf("buf len @ 40: %i\n", buf_after_40);
    assert(buf_after_40 == 360154);

    printf("buf len @ 50: %i\n", buf_after_50);
    assert(buf_after_50 == 5103798);

    free(current_buf);
    free(next_buf);

    return 0;
}
