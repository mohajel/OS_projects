CC = g++

.PHONY: all
all: read write create

create:
	$(CC) create.c -o create
	
read: read.c
	$(CC) read.c -o read

write: write.c
	$(CC) write.c -o write

.PHONY: clean
clean:
	rm  read write create
