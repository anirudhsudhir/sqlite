CC = clang

all: sqlite

sqlite: main.c
	$(CC) -o db -Wall -Werror main.c virtualmachine/virtualmachine.c compiler/compiler.c helpers/helpers.c pager/pager.c

clean:
	rm -rf db
