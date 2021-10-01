all: generateur_fichiers_test

PileTypeParentheses.o: PileTypeParentheses.c PileTypeParentheses.h
	gcc -g -Wall -c PileTypeParentheses.c

generateur_fichiers_test: generateur_fichiers_test.o PileTypeParentheses.o 
	gcc -g -Wall -o generateur_fichiers_test generateur_fichiers_test.o PileTypeParentheses.o

clean:
	rm -f generateur_fichiers_test *.o
