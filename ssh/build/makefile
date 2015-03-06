CC = g++
CXXFLAGS = -std=gnu++11

HEADERS = -I ../include
SRC_DIR = ../src
SOURCES = $(shell find $(SRC_DIR) -name "*.cpp")
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,%.o,$(SOURCES))

all: libssh_new.so

libssh_new.so: objects
	$(CC) $(CXXFLAGS) $(OBJECTS) -shared -o libssh_new.so

objects:
	$(foreach cpp,$(SOURCES),$(CC) $(CXXFLAGS) $(HEADERS) $(cpp) -lssh -c -fpic -o $(patsubst $(SRC_DIR)/%.cpp,%.o,$(cpp);))
	
clean:
	rm $(OBJECTS) libssh_new.so

