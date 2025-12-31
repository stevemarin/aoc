
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int part1(FILE *fh) {
    rewind(fh);

    enum { WIDTH = 200, HALF_WIDTH = 100 };

    int x = HALF_WIDTH, y = HALF_WIDTH;
    int map[WIDTH][WIDTH] = {0};
    map[x][y] = 1;

    int ch;
    while ((ch = fgetc(fh)) != EOF) {
        switch (ch) {
        case '<':
            x--;
            break;
        case '>':
            x++;
            break;
        case '^':
            y++;
            break;
        case 'v':
            y--;
            break;
        default:
            printf("you shouldn't be here\n");
            exit(1);
        }
        map[x][y]++;
    }

    int num_positive = 0;
    for (int ii = 0; ii < WIDTH; ii++) {
        for (int jj = 0; jj < WIDTH; jj++) {
            if (map[ii][jj] > 0) {
                num_positive++;
            }
        }
    }

    return num_positive;
}

int part2(FILE *fh) {
    rewind(fh);

    enum { WIDTH = 200, HALF_WIDTH = 100 };

    int santa_x = HALF_WIDTH, santa_y = HALF_WIDTH;
    int santa_map[WIDTH][WIDTH] = {0};
    santa_map[santa_x][santa_y] = 1;

    int robot_x = HALF_WIDTH, robot_y = HALF_WIDTH;
    int robot_map[WIDTH][WIDTH] = {0};
    robot_map[robot_x][robot_y] = 1;

    int ch;
    while ((ch = fgetc(fh)) != EOF) {
        switch (ch) {
        case '<':
            santa_x--;
            break;
        case '>':
            santa_x++;
            break;
        case '^':
            santa_y++;
            break;
        case 'v':
            santa_y--;
            break;
        default:
            printf("you shouldn't be here\n");
            exit(1);
        }
        santa_map[santa_x][santa_y]++;

        ch = fgetc(fh);
        assert(ch != EOF);
        switch (ch) {
        case '<':
            robot_x--;
            break;
        case '>':
            robot_x++;
            break;
        case '^':
            robot_y++;
            break;
        case 'v':
            robot_y--;
            break;
        default:
            printf("you shouldn't be here\n");
            exit(1);
        }
        robot_map[robot_x][robot_y]++;
    }

    int num_positive = 0;
    for (int ii = 0; ii < WIDTH; ii++) {
        for (int jj = 0; jj < WIDTH; jj++) {
            if (santa_map[ii][jj] + robot_map[ii][jj] > 0) {
                num_positive++;
            }
        }
    }

    return num_positive;
}

int main(int argc, char *argv[]) {
    assert(argc == 2);

    FILE *fh = fopen(argv[1], "r");
    if (!fh) {
        printf("cannot find file: %s", argv[1]);
        exit(1);
    }

    int res = part1(fh);
    printf("part 1 lucky houses: %i\n", res);
    res = part2(fh);
    printf("part 2 lucky houses: %i\n", res);

    return 0;
}
