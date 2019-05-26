csha:	src/main.c src/sha256const.o src/sha256Funcs.o
	
	gcc -o csha256 src/main.c build/*.o

debug: main.c 
	gcc -g -o csha256 *.c

src/sha256const.o: src/sha256const.c
	gcc -o build/sha256const.o -c src/sha256const.c

src/sha256Funcs.o: src/sha256Funcs.c
	gcc -o build/sha256Funcs.o -c src/sha256Funcs.c

.PHONY: clean

clean:
	rm build/*.o