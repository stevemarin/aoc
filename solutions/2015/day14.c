

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_REINDEER 9

typedef struct Reindeer {
    int speed;
    int exert_seconds;
    int rest_seconds;
} Reindeer;

int distance(Reindeer deer, int total_seconds) {
    int cycle_length = deer.exert_seconds + deer.rest_seconds;
    int total_cycles = total_seconds / cycle_length;
    int unexpended_seconds = total_seconds - (total_cycles * cycle_length);
    int remaining_seconds = deer.exert_seconds > unexpended_seconds ? unexpended_seconds : deer.exert_seconds;
    return deer.speed * (total_cycles * deer.exert_seconds + remaining_seconds);
}

void skip_non_numbers(FILE *fh) {
    int ch;
    do {
        ch = fgetc(fh);
    } while ((ch < '0' || '9' < ch));

    fseek(fh, -1, SEEK_CUR);
}

int read_number(FILE *fh) {
    char buf[8] = {0};

    int ch = fgetc(fh), idx = 0;
    while ('0' <= ch && ch <= '9') {
        assert(idx < 8);
        buf[idx++] = (char)ch;
        ch = fgetc(fh);
    };

    return atoi(buf);
}

Reindeer parse_line(FILE *fh) {
    // Comet can fly 13 km/s for 7 seconds, but then must rest for 82 seconds.
    skip_non_numbers(fh);
    int speed = read_number(fh);
    skip_non_numbers(fh);
    int exert_seconds = read_number(fh);
    skip_non_numbers(fh);
    int rest_seconds = read_number(fh);
    while (fgetc(fh) != '\n') {
    };

    return (Reindeer){speed, exert_seconds, rest_seconds};
}

int part1(FILE *fh) {
    int max_distance = 0;
    while (fgetc(fh) != EOF) {
        Reindeer deer = parse_line(fh);
        int current_distance = distance(deer, 2503);
        max_distance = current_distance > max_distance ? current_distance : max_distance;
    }
    return max_distance;
}

int part2(FILE *fh) {
    rewind(fh);

    // populate Reindeer array
    Reindeer deers[NUM_REINDEER] = {0};
    for (int idx = 0; idx < NUM_REINDEER; idx++) {
        deers[idx] = parse_line(fh);
    }

    int scores[NUM_REINDEER] = {0};
    int distances[NUM_REINDEER] = {0};
    for (int t = 1; t <= 2503; t++) {
        // determine the max distance at each time and distance of each reindeer at each time
        int max_distance = 0;
        for (int idx = 0; idx < NUM_REINDEER; idx++) {
            int d = distance(deers[idx], t);
            max_distance = d > max_distance ? d : max_distance;
            distances[idx] = d;
        }

        // increment reindeer in the lead
        for (int idx = 0; idx < NUM_REINDEER; idx++) {
            if (distances[idx] == max_distance) {
                scores[idx]++;
            }
        }
    }

    // get max score
    int max_score = 0;
    for (int idx = 0; idx < NUM_REINDEER; idx++) {
        int current_score = scores[idx];
        max_score = current_score > max_score ? current_score : max_score;
    }

    return max_score;
}

int main(int argc, char *argv[]) {
    assert(distance((Reindeer){14, 10, 127}, 1000) == 1120);
    assert(distance((Reindeer){16, 11, 162}, 1000) == 1056);

    assert(argc == 2);
    char *filename = argv[1];
    FILE *fh = fopen(filename, "r");
    if (!fh) {
        printf("cannot find file: %s\n", argv[1]);
        exit(1);
    }

    int max_distance = part1(fh);
    printf("max distance: %i\n", max_distance);
    assert(max_distance == 2660);

    int max_score = part2(fh);
    printf("max score: %i\n", max_score);
    assert(max_score == 1256);

    return 0;
}
