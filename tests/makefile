CC= gcc -ansi -pedantic -Wall
P= ../../Unity/src
P2= ../src/graph_implementation
P3= ../src
FLAGS= -I $(P) -I $(P3)
OBJS=test_graph_implementation.c $(P)/unity.c $(P2)/graph.o $(P2)/box.o $(P2)/vertex.o
DEPS= $(P3)/graph.h $(P3)/graph_implementation.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(FLAGS)
	
test_graph_implementation2: $(OBJS)
	$(CC) -o $@.out $^ $(FLAGS)

# this is the old make for test
test_graph_implementation: test_graph_implementation.c ../src/graph_implementation.c $(P)/unity.c
	$(CC) test_graph_implementation.c  ../src/graph_implementation.c $(P)/unity.c -o test_graph_implementation.out -I $(P)