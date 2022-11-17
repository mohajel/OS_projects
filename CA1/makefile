#In the Name of God

CC = gcc

.PHONY: all
all: buyer seller

buyer: client.c manual.h
	$(CC) client.c -o buyer

seller: server.c manual.h
	$(CC) server.c -o seller

.PHONY: clean
clean:
	rm buyer seller *.txt
