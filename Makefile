
CC = g++-7
SRC_DIR = src/
INCLUDE_DIR = include/

OUTFILE = casino

COMPILE_FLAGS = -lncurses -std=c++14 -Wall -Wextra

# -------------------------------------------------

all: bin compile build

bin:
	mkdir bin

compile:
	@rm bin/*.o; \
	cd $(SRC_DIR); \
	$(CC) -c *.cpp -I../include; \
	mv *.o ../bin; \
	cd -;

build:
	$(CC) bin/*.o $(COMPILE_FLAGS) -o $(OUTFILE)

clean:
	rm -rf bin

.PHONY: all compile build clean
