#In the Name of God

CC = gcc

main = genre_counter

.PHONY: all
all: $(main) map reduce

$(main): main.c manual.h
	$(CC) main.c -o $(main)

map: map.c manual.h
	$(CC) map.c -o map

reduce: reduce.c manual.h
	$(CC) reduce.c -o reduce

.PHONY: clean
clean:
	rm $(main) map reduce
