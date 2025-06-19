MainComp:
	g++ -Wall -fopenmp -lm main.cpp UtilsFunctions.cpp -o main.o

NewComp:
	g++ -Wall -fopenmp -lm -O2 new.cpp UtilsFunctions.cpp -o new.o

MainEx:
	./main.o

NewEx:
	./new.o

Main:
	make MainComp
	make MainEx

New:
	make NewComp
	make NewEx