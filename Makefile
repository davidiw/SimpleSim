CC = gcc
DEBUG =
CFLAGS = -c $(DEBUG)
LFLAGS = $(DEBUG)
BUILD_DIR = build
LIB_OBJS = build/sim_helper.o build/graph.o build/chord.o build/greedy.o \
					 build/sorted_list.o build/safe.o
BUILD_OBJS = $(LIB_OBJS) build/sim.o
TEST_OBJS = $(LIB_OBJS) build/cutest.o build/tests.o build/sorted_list_test.o

build/sim:
	$(CC) $(LFLAGS) $(BUILD_OBJS) -o build/sim

run: build/sim
	./build/sim

test: build/test
	./build/test

clean:
	rm -rf build

# Binaries

build/sim:

build/test: $(TEST_OBJS)
	$(CC) $(LFLAGS) $(TEST_OBJS) -o build/test
 
# Main libraries

build/graph.o: src/graph.c src/graph.h src/safe.h src/sorted_list.h
	$(CC) $(CFLAGS) src/graph.c -o build/graph.o

build/greedy.o: src/greedy.c src/greedy.h src/message.h src/sim_helper.h
	$(CC) $(CFLAGS) src/greedy.c -o build/greedy.o

build/safe.o: src/safe.c src/safe.h
	$(CC) $(CFLAGS) src/safe.c -o build/safe.o

build/chord.o: src/chord.c src/chord.h src/graph.h src/sim_helper.h
	$(CC) $(CFLAGS) src/chord.c -o build/chord.o

build/sim.o: src/sim.c src/graph.h src/safe.h src/sim_helper.h
	$(CC) $(CFLAGS) src/sim.c -o build/sim.o

build/sim_helper.o: src/sim_helper.c src/sim_helper.h
	$(CC) $(CFLAGS) src/sim_helper.c -o build/sim_helper.o

build/sorted_list.o: src/sorted_list.c src/sorted_list.h src/safe.h
	$(CC) $(CFLAGS) src/sorted_list.c -o build/sorted_list.o

# Test libraries

build/cutest.o: src/tests/CuTest.c src/tests/CuTest.h
	$(CC) $(CFLAGS) src/tests/CuTest.c -o build/cutest.o

build/tests.o: src/tests/CuTest.h src/tests/tests.c
	$(CC) $(CFLAGS) src/tests/tests.c -o build/tests.o

build/sorted_list_test.o: src/tests/CuTest.h src/sorted_list.h src/safe.h \
	src/tests/sorted_list.c 
	$(CC) $(CFLAGS) src/tests/sorted_list.c -o build/sorted_list_test.o
