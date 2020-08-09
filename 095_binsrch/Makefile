CCFLAGS=--std=gnu++98 -pedantic -Wall -Werror -ggdb3
test: test-bsearch.o search.o 
	g++ -o test $(CCFLAGS) test-bsearch.o search.o 

%.o: %.cpp function.h 
	g++ -c $(CCFLAGS) $<

clean:
	rm -f *.o  *~ test
