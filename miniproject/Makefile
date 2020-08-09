CCFLAGS=--std=c++11 -pedantic -Wall -Werror -ggdb3
efficient_frontier: main.o read.o CalculCov.o  Lagrange.o
	g++ -o efficient_frontier $(CCFLAGS) main.o read.o CalculCov.o Lagrange.o 
%.o: %.cpp read.h calculate.h 
	g++ -I ./Eigen/ CalculCov.cpp read.cpp Lagrange.cpp -c $(CCFLAGS) $<
clean:
	rm -f  *.o  *~ efficient_frontier
