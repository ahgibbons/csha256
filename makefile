csha:	main.c sha256const.o sha256Funcs.o
	gcc -o main main.c *.o

debug: main.c 
	gcc -g -o main *.c

sha256const.o: sha256const.c
	gcc -c sha256const.c

sha256Funcs.o: sha256Funcs.c
	gcc -c sha256Funcs.c

.PHONY: clean

clean:
	rm *.o