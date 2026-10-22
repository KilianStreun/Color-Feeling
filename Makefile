CC = gcc
CFLAGS = -Wall -Wextra -g $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs) -lm

main: main.c
	$(CC) $(CFLAGS) main.c -o main $(LDFLAGS)

run: main
	./main

clean:
	rm -f main
