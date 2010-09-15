CC := gcc
DEBUG := -g
CFLAGS := -c $(DEBUG) $(shell pkg-config --cflags glib-2.0)
LFLAGS := -lm $(DEBUG) $(shell pkg-config --libs glib-2.0)
LIB_OBJS := src/graph.o src/symphony.o 
BUILD_OBJS := $(LIB_OBJS) src/sim.o
TEST_OBJS := $(LIB_OBJS) src/tests/tests.o src/tests/test_graph.o

# Binaries
sim: $(BUILD_OBJS)
	$(CC) $(LFLAGS) $(BUILD_OBJS) -o $@

test: $(TEST_OBJS)
	$(CC) $(LFLAGS) $(TEST_OBJS) -o $@
 
# Libraries
src/%.o : src/%.c
	$(CC) $(CFLAGS) $< -o $@

src/%/%.o: src/tests/%.c
	$(CC) $(CFLAGS) $< -o $@

# Helpers
run: sim
	./sim

run_test: test
	./test

clean:
	rm -rf $(BUILD_OBJS) $(TEST_OBJS) sim test
