CC = gcc
CFLAGS = -Wall -pedantic -fsanitize=address

test:
	$(CC) $(CFLAGS) test.c && ./a.out

main:
	$(CC) $(CFLAGS) main.c && ./a.out



