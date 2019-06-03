csha:	src/main.c build/sha256const.o build/sha256Funcs.o build/
	gcc -O2 -o csha256 src/main.c build/*.o

debug: src/main.c 
	gcc -g -o dcsha256 src/*.c

build/:
	mkdir build

build/sha256const.o: src/sha256const.c build/
	gcc -O2 -o build/sha256const.o -c src/sha256const.c

build/sha256Funcs.o: src/sha256Funcs.c build/
	gcc -O2 -o build/sha256Funcs.o -c src/sha256Funcs.c

.PHONY: clean

clean:
	rm build/*.o
