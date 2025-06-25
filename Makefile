MainComp:
	g++ -Wall -fopenmp -lm main.cpp UtilsFunctions.cpp -o main.o

NewComp: #Borrar
	g++ -Wall -fopenmp -lm -O2 new.cpp UtilsFunctions.cpp -o new.o

MainEx: 
	./main.o

NewEx: #Borrar
	./new.o

Main: 
	make MainComp
	make MainEx

New: #Borrar
	make NewComp
	make NewEx

Clear:
	rm *.o

All:
	make Clear
	make MainComp
	make MainEx

.DEFAULT_GOAL = All