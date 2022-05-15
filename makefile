CC=gcc
CFLAGS=-Wall -ggdb

all: nist_867 nist_868

nist_867: nist_867.c
	$(CC) $(CFLAGS) -o nist_867 nist_867.c

nist_868: nist_868.c
	$(CC) $(CFLAGS) -o nist_868 nist_868.c

clean:
	rm -rf nist_867 nist_868
