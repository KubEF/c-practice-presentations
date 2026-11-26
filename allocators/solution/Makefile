CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: all clean
all: app

app: main.c bump.c bump.h
	$(CC) $(CFLAGS) main.c bump.c -o app

clean:
	rm -f app
