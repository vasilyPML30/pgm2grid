all: bin convert
convert: bin/main.o bin/convert.o
	g++ -g bin/convert.o bin/main.o -o convert
bin/main.o: src/main.cpp include/convert.h 
	g++ -c -std=c++11 -pedantic -Wall -Wextra -I ./include src/main.cpp -o bin/main.o
bin/convert.o: src/convert.cpp  include/convert.h
	g++ -c -std=c++11 -pedantic -Wall -Wextra -I ./include src/convert.cpp -o bin/convert.o
clean:
	rm -rf bin convert
bin:
	mkdir -p bin
