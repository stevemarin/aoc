
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF 32

#define NUM_PLACES 8
char places[NUM_PLACES][32] = {"AlphaCentauri", "Arbre",      "Faerun", "Norrath",
                               "Snowdin",       "Straylight", "Tambi",  "Tristram"};

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

void zero_buf(char buf[BUF]) {
    for (int i = 0; i < BUF; i++) {
        buf[i] = 0;
    }
}

_Bool read_into_buf(FILE *fh, char buf[BUF]) {
    zero_buf(buf);
    for (int idx = 0; idx < BUF; idx++) {
        int ch = fgetc(fh);
        if (ch == EOF) {
            if (idx == 0) {
                return true;
            } else {
                printf("hit eof inside a line");
                exit(1);
            }
        }
        if (ch == ' ' || ch == '\n') {
            break;
        }
        assert (CHAR_MIN <= ch && ch < CHAR_MAX);
        buf[idx] = (char)ch;
    }
    return false;
}

int place_to_idx(char *place) {
    for (int idx = 0; idx < NUM_PLACES; idx++) {
        if (strcmp(places[idx], place) == 0) {
            return idx;
        }
    }
    printf("unknown place: %s\n", place);
    exit(1);
}

void parse_input(FILE *fh, int dists[NUM_PLACES][NUM_PLACES]) {
    while (true) {
        char buf[BUF] = {0};
        _Bool hit_eof = read_into_buf(fh, buf);
        if (hit_eof) {
            break;
        }
        int src = place_to_idx(buf);

        assert(fgetc(fh) == 't');
        assert(fgetc(fh) == 'o');
        assert(fgetc(fh) == ' ');

        read_into_buf(fh, buf);
        int dest = place_to_idx(buf);

        assert(fgetc(fh) == '=');
        assert(fgetc(fh) == ' ');

        read_into_buf(fh, buf);
        int dist = atoi(buf);

        dists[src][dest] = dist;
        dists[dest][src] = dist;
    }
}

int main(int argc, char *argv[]) {
    assert(argc == 2);
    FILE *fh = fopen(argv[1], "r");
    if (!fh) {
        printf("cannot find file: %s", argv[1]);
        exit(1);
    }

    // populate distance matrix
    int distances[NUM_PLACES][NUM_PLACES] = {0};
    parse_input(fh, distances);

    // initialize indices
    int indices[NUM_PLACES];
    for (int idx = 0; idx < NUM_PLACES; idx++) {
        indices[idx] = idx;
    }

    // do first permutation
    int min_distance = 0;
    int prev_idx = indices[0];
    for (int idx = 1; idx < NUM_PLACES; idx++) {
        int current_idx = indices[idx];
        min_distance += distances[prev_idx][current_idx];
        prev_idx = current_idx;
    }
    int max_distance = min_distance;

    // now do rest of the permutations
    do {
        next_permutation(indices, NUM_PLACES);
        if (indices[0] > indices[NUM_PLACES - 1]) {
            continue;
        } else if (monotonic(indices, NUM_PLACES)) {
            break;
        }

        int distance = 0;
        int prev_idx = indices[0];
        for (int idx = 1; idx < NUM_PLACES; idx++) {
            int current_idx = indices[idx];
            distance += distances[prev_idx][current_idx];
            prev_idx = current_idx;
        }
        min_distance = min_distance < distance ? min_distance : distance;
        max_distance = max_distance > distance ? max_distance : distance;

    } while (!monotonic(indices, NUM_PLACES));

    printf("min distance: %i\n", min_distance);
    printf("max distance: %i\n", max_distance);

    return 0;
}