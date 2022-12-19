# # In the Name of God

BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = includes

CC = g++ 
CFLAGS = --std=c++11 -I$(INCLUDE_DIR) -pthread

EXECUTABLE_FILE = imageFilters.out

OBJECTS = \
	$(BUILD_DIR)/bmp24.o \
	$(BUILD_DIR)/thread.o \
	$(BUILD_DIR)/reverse.o \
	$(BUILD_DIR)/emboss.o \
	$(BUILD_DIR)/dimond.o\
	$(BUILD_DIR)/main.o

all: clean $(BUILD_DIR) $(EXECUTABLE_FILE)

$(EXECUTABLE_FILE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE_FILE)

$(BUILD_DIR):
	mkdir build

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(INCLUDE_DIR)/defs.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.cpp -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/bmp24.o: $(SRC_DIR)/bmp24.cpp $(INCLUDE_DIR)/bmp24.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/bmp24.cpp -o $(BUILD_DIR)/bmp24.o

$(BUILD_DIR)/dimond.o: $(SRC_DIR)/dimond.cpp $(INCLUDE_DIR)/dimond.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/dimond.cpp -o $(BUILD_DIR)/dimond.o

$(BUILD_DIR)/emboss.o: $(SRC_DIR)/emboss.cpp $(INCLUDE_DIR)/emboss.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/emboss.cpp -o $(BUILD_DIR)/emboss.o

$(BUILD_DIR)/reverse.o: $(SRC_DIR)/reverse.cpp $(INCLUDE_DIR)/reverse.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/reverse.cpp  -o $(BUILD_DIR)/reverse.o

$(BUILD_DIR)/thread.o: $(SRC_DIR)/thread.cpp $(INCLUDE_DIR)/thread.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/thread.cpp -o $(BUILD_DIR)/thread.o

.PHONY: clean all
clean:
	rm -rf $(BUILD_DIR) *.o *.out outputImg.bmp