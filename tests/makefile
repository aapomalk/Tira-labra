CC= gcc -ansi -pedantic -Wall
P= ../../Unity/src
P2= ../src/graph_implementation
P3= ../src
P4= ../src/a_star_implementation

FLAGS= -I $(P) -I $(P3)
OBJS=test_graph_implementation.c $(P)/unity.c $(P2)/graph.o $(P2)/box.o $(P2)/vertex.o
DEPS= $(P3)/graph.h $(P3)/graph_implementation.h $(P3)/a_star.h $(P4)/heap.h $(P4)/a_star_implementation.h

OBJS2=test_heap.c $(P)/unity.c $(P4)/heap.o
FLAGS2= -I $(P4)

OBJS3=test_a_star.c $(P)/unity.c $(P4)/a_star.o $(P2)/graph.o $(P2)/box.o $(P2)/vertex.o $(P4)/heap.o

OBJS_X=$(P)/unity.c $(P4)/a_star.o $(P2)/graph.o $(P2)/box.o $(P2)/vertex.o $(P4)/heap.o

OBJS_Y=$(P)/unity.c $(P4)/a_star.o $(P2)/graph_zero.o $(P2)/box.o $(P2)/vertex.o $(P4)/heap.o

performance: test_performance test_performance_zero

test_performance: test_performance.c $(OBJS_X)
	$(CC) -o $@.out $^ $(FLAGS) $(FLAGS2)
#	./$@.out 10 5
#	./$@.out 16 8

test_performance_zero: test_performance.c $(OBJS_Y)
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

$(P2)/graph_zero.o: $(P2)/graph.c $(DEPS)
	$(CC) -c -o $@ $< $(FLAGS) -D HEURISTIC_ZERO

# this is the old make for test
test_graph_implementation: test_graph_implementation.c ../src/graph_implementation.c $(P)/unity.c
	$(CC) test_graph_implementation.c  ../src/graph_implementation.c $(P)/unity.c -o test_graph_implementation.out -I $(P)
