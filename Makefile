
SHELL:=/bin/bash

CC := gcc
CFLAGS := -O3 -Wall -Wextra -Wfloat-equal -Wundef -Wpointer-arith -Wstrict-prototypes  \
          -Wwrite-strings -Wcast-qual -Wswitch-enum -Wswitch-default -Wconversion \
		  -Wunreachable-code -Wstrict-overflow=5 -fno-strict-overflow

DAY := $(shell echo "0$(day)" | tail -c 3)
YEAR := $(shell echo "20$(year)" | tail -c 5)

run: solutions/$(YEAR)/day$(DAY)
	@chmod a+x a.out && ./a.out data/$(YEAR)/day$(DAY).txt

sample: solutions/$(YEAR)/day$(DAY)
	@chmod a+x a.out && ./a.out data/$(YEAR)/day$(DAY)_sample.txt

solutions/$(YEAR)/day$(DAY):
	@$(CC) $(CFLAGS) -O3 solutions/$(YEAR)/day$(DAY).c