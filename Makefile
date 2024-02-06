CC = clang

all: sqlite

sqlite: main.c
	$(CC) -o sqlite -Wall main.c

clean:
	rm -rf sqlite
