CC = clang

all: sqlite

sqlite: main.c
	$(CC) -o db -Wall main.c virtualmachine/virtualmachine.c compiler/compiler.c helpers/helpers.c

clean:
	rm -rf db
