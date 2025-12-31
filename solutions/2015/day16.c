
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Sue {
    int children;
    int cats;
    int samoyeds;
    int pomeranians;
    int akitas;
    int vizslas;
    int goldfish;
    int trees;
    int cars;
    int perfumes;
} Sue;

Sue correct = {3, 7, 2, 3, 0, 0, 5, 3, 2, 1};

bool match_sues(Sue correct, Sue current) {
    if (correct.akitas != current.akitas && current.akitas != -1) {
        return false;
    } else if (correct.children != current.children && current.children != -1) {
        return false;
    } else if (correct.cars != current.cars && current.cars != -1) {
        return false;
    } else if (correct.cats != current.cats && current.cats != -1) {
        return false;
    } else if (correct.goldfish != current.goldfish && current.goldfish != -1) {
        return false;
    } else if (correct.perfumes != current.perfumes && current.perfumes != -1) {
        return false;
    } else if (correct.pomeranians != current.pomeranians && current.pomeranians != -1) {
        return false;
    } else if (correct.samoyeds != current.samoyeds && current.samoyeds != -1) {
        return false;
    } else if (correct.trees != current.trees && current.trees != -1) {
        return false;
    } else if (correct.vizslas != current.vizslas && current.vizslas != -1) {
        return false;
    }
    return true;
}

bool match_sues_ranges(Sue correct, Sue current) {
    if (correct.akitas != current.akitas && current.akitas != -1) {
        return false;
    } else if (correct.children != current.children && current.children != -1) {
        return false;
    } else if (correct.cars != current.cars && current.cars != -1) {
        return false;
    } else if (correct.cats >= current.cats && current.cats != -1) {
        return false;
    } else if (correct.goldfish <= current.goldfish && current.goldfish != -1) {
        return false;
    } else if (correct.perfumes != current.perfumes && current.perfumes != -1) {
        return false;
    } else if (correct.pomeranians <= current.pomeranians && current.pomeranians != -1) {
        return false;
    } else if (correct.samoyeds != current.samoyeds && current.samoyeds != -1) {
        return false;
    } else if (correct.trees >= current.trees && current.trees != -1) {
        return false;
    } else if (correct.vizslas != current.vizslas && current.vizslas != -1) {
        return false;
    }
    return true;
}

int read_number(FILE *fh) {
    char ch[8] = {0};

    for (int idx = 0; idx <= 8; idx++) {
        int next = fgetc(fh);
        if (next < '0' || '9' < next) {
            break;
        }
        ch[idx] = (char)next;
    }

    fseek(fh, -1, SEEK_CUR);
    return atoi(ch);
}

bool check_newline_else_skip(FILE *fh) {
    int ch = fgetc(fh);
    if (ch == '\n') {
        return true;
    } else {
        assert(ch == ',');
        assert(fgetc(fh) == ' ');
        return false;
    }
    assert(false);
}

void skip_until(FILE *fh, int ch) {
    int next = fgetc(fh);
    while (next != ch) {
        next = fgetc(fh);
    }
}

Sue parse_line(FILE *fh) {
    // Sue 2: akitas: 9, children: 3, samoyeds: 9
    // Sue 3: trees: 6, cars: 6, children: 4

    Sue sue = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    skip_until(fh, ':');
    fgetc(fh);

    while (true) {
        char chs[3] = {};
        chs[0] = (char)fgetc(fh);
        chs[1] = (char)fgetc(fh);
        chs[2] = (char)fgetc(fh);
        skip_until(fh, ' ');
        int value = read_number(fh);

        if (chs[0] == 'a') {
            sue.akitas = value;
        } else if (chs[0] == 'c' && chs[2] == 'i') {
            sue.children = value;
        } else if (chs[0] == 'c' && chs[2] == 'r') {
            sue.cars = value;
        } else if (chs[0] == 'c' && chs[2] == 't') {
            sue.cats = value;
        } else if (chs[0] == 'g') {
            sue.goldfish = value;
        } else if (chs[0] == 'p' && chs[1] == 'e') {
            sue.perfumes = value;
        } else if (chs[0] == 'p' && chs[1] == 'o') {
            sue.pomeranians = value;
        } else if (chs[0] == 's') {
            sue.samoyeds = value;
        } else if (chs[0] == 't') {
            sue.trees = value;
        } else if (chs[0] == 'v') {
            sue.vizslas = value;
        } else {
            printf("unknown value: %s", chs);
            exit(1);
        }
        if (check_newline_else_skip(fh)) {
            return sue;
        }
    }

    assert(false);
}

int main(int argc, char *argv[]) {
    assert(argc == 2);
    FILE *fh = fopen(argv[1], "r");
    if (fh == nullptr) {
        printf("cannot open file: %s", argv[1]);
        exit(1);
    }

    int ch = 0;
    int idx = 0;
    Sue sues[500] = {0};
    while ((ch = fgetc(fh)) != EOF) {
        fseek(fh, -1, SEEK_CUR);
        Sue sue = parse_line(fh);
        sues[idx++] = sue;
    }

    bool found = false;
    for (int idx = 0; idx < 500; idx++) {
        Sue current = sues[idx];
        if (match_sues(correct, current)) {
            printf("Sue #%i matches part 1\n", idx + 1);
            found = true;
            assert(idx + 1 == 373);
        }
    }
    assert (found);

    found = false;
    for (int idx = 0; idx < 500; idx++) {
        Sue current = sues[idx];
        if (match_sues_ranges(correct, current)) {
            printf("Sue #%i matches part 2\n", idx + 1);
            found = true;
            assert(idx + 1 == 260);
        }
    }
    assert (found);

    return 0;
}