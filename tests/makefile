sCC= gcc -ansi -pedantic -Wall
P= ../../Unity/src
P2= ../src/graph_implementation
P3= ../src
P4= ../src/a_star_implementation
P5= ../src/allocation_implementation
P6= ../src/build_graph_implementation

GBV=$(P2)/graph.o $(P2)/box.o $(P2)/vertex.o
UNI=$(P)/unity.c
ALLOC=$(P5)/allocation.o

FLAGS= -I $(P) -I $(P3) -lm
OBJS=test_graph_implementation.c $(UNI) $(GBV) $(ALLOC)
DEPS= $(P3)/graph.h $(P3)/graph_implementation.h $(P3)/a_star.h $(P4)/heap.h $(P4)/a_star_implementation.h $(P3)/allocation.h $(P3)/build_graph_implementation.h $(P3)/build_graph.h

OBJS2=test_heap.c $(UNI) $(P4)/heap.o $(ALLOC)
FLAGS2= -I $(P4)

OBJS3=test_a_star.c $(UNI) $(P4)/a_star.o $(P2)/graph.o $(ALLOC) $(P2)/box.o $(P2)/vertex.o $(P4)/heap.o

OBJS_X=$(UNI) $(P4)/a_star.o $(GBV) $(P4)/heap.o $(ALLOC)

OBJS_Y=$(UNI) $(P4)/a_star_zero.o $(GBV) $(P4)/heap.o $(ALLOC)

OBJS_B=$(P6)/build_graph_implementation.o $(GBV) $(UNI) $(ALLOC)

all: performance test_a_star test_heap test_graph_implementation2 test_build_graph

test_build_graph: test_build_graph_implementation.c $(OBJS_B)
	$(CC) -o $@.out $^ $(FLAGS) $(FLAGS2)
	./$@.out

performance: test_performance test_performance_zero test_performance2 test_performance_zero2

test_performance: test_performance.c $(OBJS_X)
	$(CC) -o $@.out $^ $(FLAGS) $(FLAGS2)
#	./$@.out 10 5
#	./$@.out 16 8

test_performance_zero: test_performance.c $(OBJS_Y)
	$(CC) -o $@_heuristic.out $^ $(FLAGS) $(FLAGS2) 
#	./$@_heuristic_zero.out 10 5
#	./$@_heuristic_zero.out 16 8

test_performance2: test_performance2.c $(OBJS_X)
	$(CC) -o $@.out $^ $(FLAGS) $(FLAGS2)
#	./$@.out 10 5
#	./$@.out 16 8

test_performance_zero2: test_performance2.c $(OBJS_Y)
	$(CC) -o $@_heuristic.out $^ $(FLAGS) $(FLAGS2) 
#	./$@_heuristic_zero.out 10 5
#	./$@_heuristic_zero.out 16 8


test_all_tests: test_a_star test_heap test_graph_implementation2

test_a_star: $(OBJS3)
	$(CC) -o $@.out $^ $(FLAGS) $(FLAGS2)
	./$@.out

test_heap: $(OBJS2)
	$(CC) -o $@.out $^ $(FLAGS) $(FLAGS2)
	./$@.out

test_graph_implementation2: $(OBJS)
	$(CC) -o $@.out $^ $(FLAGS)
	./$@.out

#graph.o: $(P2)/graph.c $(P2)/vertex.c $(P2)/box.c
#	$(CC) -c -o $@ $^ $(FLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(FLAGS)

$(P4)/a_star_zero.o: $(P4)/a_star.c $(DEPS)
	$(CC) -c -o $@ $< $(FLAGS) -D HEURISTIC_ZERO

# this is the old make for test
test_graph_implementation: test_graph_implementation.c ../src/graph_implementation.c $(UNI)
	$(CC) test_graph_implementation.c  ../src/graph_implementation.c $(UNI) -o test_graph_implementation.out -I $(P)

just_a_test: test.c
	$(CC) test.c -o test.out
