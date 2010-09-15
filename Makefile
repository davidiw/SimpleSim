CC = gcc
DEBUG = -g
CFLAGS = -c $(DEBUG) $(shell pkg-config --cflags glib-2.0)
LFLAGS = -lm $(DEBUG) $(shell pkg-config --libs glib-2.0)
BUILD_DIR = build
LIB_OBJS = build/graph.o build/symphony.o 
BUILD_OBJS = $(LIB_OBJS) build/sim.o
TEST_OBJS = $(LIB_OBJS) build/tests.o build/test_graph.o

build/sim: $(BUILD_OBJS)
	$(CC) $(LFLAGS) $(BUILD_OBJS) -o build/sim

run: build/sim
	./build/sim

test: build/test
	./build/test

clean:
	rm -rf build

# Binaries

build/test: $(TEST_OBJS)
	$(CC) $(LFLAGS) $(TEST_OBJS) -o build/test
 
# Main libraries

build/graph.o: src/graph.c src/graph.h
	$(CC) $(CFLAGS) src/graph.c -o build/graph.o

build/chord.o: src/chord.c src/chord.h src/graph.h
	$(CC) $(CFLAGS) src/chord.c -o build/chord.o

build/symphony.o: src/symphony.c src/symphony.h src/graph.h
	$(CC) $(CFLAGS) src/symphony.c -o build/symphony.o

build/sim.o: src/sim.c src/graph.h
	$(CC) $(CFLAGS) src/sim.c -o build/sim.o

# Test libraries

build/test_graph.o: src/tests/graph.c src/graph.h
	$(CC) $(CFLAGS) src/tests/graph.c -o build/test_graph.o

build/tests.o: src/tests/tests.c
	$(CC) $(CFLAGS) src/tests/tests.c -o build/tests.o
