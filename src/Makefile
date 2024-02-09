CC = clang

all: sqlite

sqlite: main.c
	$(CC) -o sqlite -Wall main.c virtualmachine/virtualmachine.c compiler/compiler.c helpers/helpers.c

clean:
	rm -rf sqlite
