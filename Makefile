MainComp:
	g++ -Wall -fopenmp -lm main.cpp -o main.o

NewComp:
	g++ -Wall -fopenmp -lm new.cpp UtilsFunctions -o new.o

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