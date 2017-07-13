CPP_COMPILER = g++
CPP_FLAGS = -Wall -std=c++11
BUILD_DIR = ./bin/
TEST_DIR = ./test/
SRC_DIR = ./src/
HEADERS = ./headers/
DIFF = ./diff/

all: diff

diff: diff_main.cpp $(DIFF)*
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(DIFF)headers diff_main.cpp getopt_pp.cpp $(DIFF)/src/*.cpp -o $(BUILD_DIR)diff

diffusion_path: ./bin/diff.exe
	./bin/diff -n 10000000 -b > path1.dat &
	./bin/diff -n 10000000 -b > path2.dat &
	./bin/diff -n 10000000 -b > path3.dat &
	./bin/diff -n 10000000 -b > path4.dat &
	./bin/diff -n 10000000 -b > path5.dat &
	./bin/diff -n 10000000 -b > path6.dat &
	./bin/diff -n 10000000 -b > path7.dat &
	./bin/diff -n 10000000 -b > path8.dat &
	./bin/diff -n 10000000 -b > path9.dat