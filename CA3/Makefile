# # In the Name of God

CC = gcc
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = includes
EXECUTABLE_FILE = imageFilters.out
CFLAGS = -I$(INCLUDE_DIR) -pthread

OBJECTS = \
	$(BUILD_DIR)/Main.o \
	$(BUILD_DIR)/Server.o \
	$(BUILD_DIR)/Project.o \
	$(BUILD_DIR)/Utils.o

all: $(BUILD_DIR) $(EXECUTABLE_FILE)

Server: $(BUILD_DIR) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE_FILE)

$(BUILD_DIR):
	mkdir build

$(BUILD_DIR)/Main.o: $(SRC_DIR)/Main.c $(INCLUDE_DIR)/Server.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/Main.c -o $(BUILD_DIR)/Main.o

# $(BUILD_DIR)/Server.o: $(SRC_DIR)/Server.c $(INCLUDE_DIR)/Server.h $(INCLUDE_DIR)/Utils.h
# 	$(CC) $(CFLAGS) -c $(SRC_DIR)/Server.c -o $(BUILD_DIR)/Server.o

# $(BUILD_DIR)/Project.o: $(SRC_DIR)/Project.c $(INCLUDE_DIR)/Project.h
# 	$(CC) $(CFLAGS) -c $(SRC_DIR)/Project.c -o $(BUILD_DIR)/Project.o

# $(BUILD_DIR)/Utils.o: $(SRC_DIR)/Utils.c $(INCLUDE_DIR)/Utils.h
# 	$(CC) $(CFLAGS) -c $(SRC_DIR)/Utils.c -o $(BUILD_DIR)/Utils.o

.PHONY: clean all
clean:
	rm -rf $(BUILD_DIR) *.o *.out