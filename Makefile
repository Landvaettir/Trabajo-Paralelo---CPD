MainComp:
	g++ -std=c++17 -Wall -fopenmp -lm -O3 main.cpp UtilsFunctions.cpp -o main.o

MainEx: 
	./main.o

Main: 
	g++ -std=c++17 -Wall -fopenmp -lm -O3 main.cpp UtilsFunctions.cpp -o main.o


Clear:
	rm *.o *.txt

All:
	make Clear
	make MainComp
	make MainEx

.DEFAULT_GOAL = Main
