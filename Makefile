GCC=g++
CFLAGS=-std=c++11

build: kevin

kevin: main.o
	$(GCC) $^ -o $@

main.o: main.cpp
	$(GCC) $(CFLAGS) -c $^

clean:
	rm -rf *.o kevin

run: kevin
	./kevin $(var)