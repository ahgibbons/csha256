csha:	src/main.c src/sha256const.o src/sha256Funcs.o
	gcc -O2 -o csha256 src/main.c build/*.o

debug: src/main.c 
	gcc -g -o dcsha256 src/*.c

src/sha256const.o: src/sha256const.c
	gcc -O2 -o build/sha256const.o -c src/sha256const.c

src/sha256Funcs.o: src/sha256Funcs.c
	gcc -O2 -o build/sha256Funcs.o -c src/sha256Funcs.c

.PHONY: clean

clean:
	rm build/*.o
