MainComp:
	g++ -std=c++17 -Wall -fopenmp -lm -O3 main.cpp UtilsFunctions.cpp -o main.o

MainEx: 
	./main.o

Main: 
	MainComp
	MainEx

Clear:
	rm *.o *.txt

.DEFAULT_GOAL = Main
