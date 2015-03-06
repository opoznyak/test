CC = g++
CXXFLAGS = -std=gnu++11

HEADERS = -I ../include -I ../../ssh/include
SRC_DIR = ../src
LIBS = -L ../../ssh/build

all: a

a: libssh_new.so
	$(CC) $(CXXFLAGS) $(HEADERS) ../src/a.cpp $(LIBS) -lssh_new -o a

libssh_new.so:
	$(MAKE) -C ../../ssh/build	
	
clean:
	rm a
	$(MAKE) -C ../../ssh/build clean