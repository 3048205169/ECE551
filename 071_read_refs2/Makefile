CPPFLAGS=-std=gnu++98 -pedantic -Wall -Werror -ggdb3
ref: ref.o 
	g++ -o ref ref.o 
ref.o: ref.cpp
	g++ $(CPPFLAGS) -c ref.cpp

.PHONY: clean
clean:
	rm -f *.o *~ 
