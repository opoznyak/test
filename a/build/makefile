PWD = $(shell pwd)

HEADERS = -I $(PWD)/../include -I $(PWD)/../../ssh/include
LIBS = -L $(PWD)/../../ssh/build

all: libssh_new.so
	g++ -std=gnu++11 $(HEADERS) ../src/a.cpp $(LIBS) -lssh_new -o a
clean:
	$(MAKE) -C $(PWD)/../../ssh/build clean	
	rm a
