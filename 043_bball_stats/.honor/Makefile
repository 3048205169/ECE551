CFLAGS=-std=gnu99 -ggdb3 -pedantic -Wall -Werror

printStats: printStats.o stats.o
	gcc -o printStats printStats.o stats.o
printGamePerformance: printGamePerformance.o stats.o
	gcc -o printGamePerformance printGamePerformance.o stats.o
addGameStats: addGameStats.o stats.o
	gcc -o addGameStats addGameStats.o stats.o
printBreakout: printBreakout.o stats.o
	gcc -o printBreakout printBreakout.o stats.o
%.o: %.c
	gcc -c $(CFLAGS) $<

.PHONY: clean
clean:
	rm -f *~ printStats printGamePerformance addGameStats printBreakout
