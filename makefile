CC = g++
CCFLAGS = -c -Wall -std=c++14
EXENAME = M05

default: main.o Topograph.o Bmp.o
	$(CC)  main.o Topograph.o Bmp.o -o $(EXENAME)
main.o: main.cpp
	$(CC) $(CCFLAGS) main.cpp
Topograph.o: Topograph.cpp
	$(CC) $(CCFLAGS) Topograph.cpp
Bmp.o: Bmp.cpp
	$(CC) $(CCFLAGS) Bmp.cpp
clean:
	rm *o $(EXENAME)
run:
	./$(EXENAME)
