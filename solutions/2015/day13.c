
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_PEOPLE 8

int first_letter_to_index(int first) {
    int val = first - 'A';
    switch (val) {
    case 0 ... 6:
        return val;
    case 12:
        return 7;
    default:
        printf("unexpected name: '%c'\n", first);
        exit(1);
    }

    assert(false);
}

int read_number(FILE *fh) {
    char buf[8] = {0};
    for (int idx = 0; idx < 8; idx++) {
        int ch = fgetc(fh);
        switch (ch) {
        case '0' ... '9':
            buf[idx] = (char)ch;
            break;
        case ' ':
            return atoi(buf);
        default:
            printf("expect digit, got '%c'\n", ch);
            exit(1);
        }
    }

    assert(false);
}

void read_through_char(FILE *fh, int through) {
    int ch = 0;
    while ((ch = fgetc(fh)) != EOF) {
        if (ch == through) {
            return;
        }
    }

    assert(false);
}

void populate_line(FILE *fh, int ch, int mat[NUM_PEOPLE][NUM_PEOPLE]) {
    int first = first_letter_to_index(ch);
    read_through_char(fh, ' '); // rest of first name

    read_through_char(fh, ' '); // would

    int sign = fgetc(fh);
    assert(sign == 'l' || sign == 'g');
    sign = sign == 'l' ? -1 : 1;
    read_through_char(fh, ' '); // ain or ose

    int val = read_number(fh);

    read_through_char(fh, ' '); // happiness
    read_through_char(fh, ' '); // units
    read_through_char(fh, ' '); // by
    read_through_char(fh, ' '); // sitting
    read_through_char(fh, ' '); // next
    read_through_char(fh, ' '); // to

    int second = first_letter_to_index(fgetc(fh));
    read_through_char(fh, '\n'); // rest of second name

    assert(0 <= first && first < NUM_PEOPLE);
    assert(0 <= second && second < NUM_PEOPLE);
    assert(first != second);

    mat[first][second] = sign * val;
}

_Bool monotonic(int *vals, int num_vals) {
    if (num_vals < 0) {
        printf("cannot have negative number of values");
        exit(1);
    } else if (num_vals < 2) {
        return true;
    }

    int prev = vals[0];
    for (int idx = 1; idx < num_vals; idx++) {
        int current = vals[idx];
        if (current <= prev) {
            return false;
        }
        prev = current;
    }
    return true;
}

// algorithm from
// medium.com/@slicesharpy/understanding-the-next-permutation-problem-from-naive-to-optimal-55fbbff8a94f
void next_permutation(int *nums, int length) {
    assert(length > 2);
    int idx = length - 2;
    while (idx >= 0 && nums[idx] >= nums[idx + 1]) {
        idx--;
    }

    if (idx >= 0) {
        int jdx = length - 1;
        while (nums[jdx] <= nums[idx]) {
            jdx--;
        }
        int nums_idx = nums[idx];
        nums[idx] = nums[jdx];
        nums[jdx] = nums_idx;
    }

    int left = idx + 1;
    int right = length - 1;
    while (left < right) {
        int nums_left = nums[left];
        nums[left] = nums[right];
        nums[right] = nums_left;
        left++;
        right--;
    }
}

int part1(int mat[NUM_PEOPLE][NUM_PEOPLE]) {
    int nums[NUM_PEOPLE - 1] = {0};
    for (int idx = 0; idx < NUM_PEOPLE - 1; idx++) {
        nums[idx] = idx;
    }

    int max_score = INT_MIN;
    do {
        int score = 0;

        for (int idx = 0; idx < NUM_PEOPLE - 2; idx++) {
            int current = nums[idx];
            int next = nums[idx + 1];
            score += mat[current][next] + mat[next][current];
        }

        int second_to_last = nums[NUM_PEOPLE - 2];
        int last = NUM_PEOPLE - 1;
        score += mat[second_to_last][last] + mat[last][second_to_last];

        int first = nums[0];
        score += mat[first][last] + mat[last][first];

        max_score = score > max_score ? score : max_score;

        next_permutation(nums, NUM_PEOPLE - 1);
    } while (!monotonic(nums, NUM_PEOPLE - 1));

    return max_score;
}

// same as just ignoring wrap-around
int part2(int mat[NUM_PEOPLE][NUM_PEOPLE]) {
    int nums[NUM_PEOPLE] = {0};
    for (int idx = 0; idx < NUM_PEOPLE; idx++) {
        nums[idx] = idx;
    }

    int max_score = INT_MIN;
    do {
        int score = 0;
        for (int idx = 0; idx < NUM_PEOPLE - 1; idx++) {
            int current = nums[idx];
            int next = nums[idx + 1];
            score += mat[current][next] + mat[next][current];
        }

        max_score = score > max_score ? score : max_score;

        next_permutation(nums, NUM_PEOPLE);
    } while (!monotonic(nums, NUM_PEOPLE));

    return max_score;
}

int main(int argc, char *argv[]) {
    assert(argc == 2);
    FILE *fh = fopen(argv[1], "r");
    if (!fh) {
        printf("cannot find file: %s\n", argv[1]);
        exit(1);
    }

    int mat[NUM_PEOPLE][NUM_PEOPLE] = {0};

    int ch = 0;
    while ((ch = fgetc(fh)) != EOF) {
        populate_line(fh, ch, mat);
    }

    int score = part1(mat);
    printf("SCORE = %i\n", score);
    assert(score == 733);

    score = part2(mat);
    printf("SCORE = %i\n", score);
    assert(score == 725);

    return 0;
}