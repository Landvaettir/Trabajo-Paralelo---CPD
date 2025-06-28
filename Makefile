MainComp:
	g++ -Wall -fopenmp -lm main.cpp UtilsFunctions.cpp -o main.o

MainEx: 
	./main.o

Main: 
	make MainComp
	make MainEx

Clear:
	rm *.o

All:
	make Clear
	make MainComp
	make MainEx

.DEFAULT_GOAL = All