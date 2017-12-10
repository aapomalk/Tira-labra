CC= gcc -g -ansi -pedantic -Wall
P= ../../Unity/src
P2= ../src/graph_implementation
P3= ../src
P4= ../src/a_star_implementation
P5= ../src/allocation_implementation
P6= ../src/build_graph_implementation
P7= ../src/header_files

GBV=$(P2)/graph.o $(P2)/box.o $(P2)/vertex.o
UNI=$(P)/unity.c
ALLOC=$(P5)/allocation.o

FLAGS= -I $(P) -I $(P3) -I $(P7) -lm
OBJS=test_graph_implementation.c $(UNI) $(GBV) $(ALLOC)

DEPS= $(P7)/graph.h $(P7)/graph_implementation.h $(P7)/a_star.h \
$(P4)/heap.h $(P4)/a_star_implementation.h $(P7)/allocation.h \
$(P7)/build_graph_implementation.h $(P7)/build_graph.h

OBJS2=test_heap.c $(UNI) $(P4)/heap.o $(ALLOC)
FLAGS2= -I $(P4)

A_STAR=$(P4)/a_star.o  $(P4)/heap.o

OBJS3=test_a_star.c $(UNI) $(A_STAR) $(ALLOC) \
$(GBV)

OBJS_X=$(UNI) $(P4)/a_star.o $(GBV) $(P4)/heap.o $(ALLOC)

OBJS_Y=$(UNI) $(P4)/a_star_zero.o $(GBV) $(P4)/heap.o $(ALLOC)

COMBINED= $(GBV) $(UNI) $(ALLOC)

OBJS_B=$(P6)/read_pdb.o $(COMBINED)
OBJS_C=$(P6)/read_parameters.o $(COMBINED)
OBJS_D=$(P6)/read_pdb.o $(COMBINED) $(P6)/read_parameters.o

dir2=/opt/gromacs-5.0.7/lib
dir3=/opt/gromacs-5.0.7/include/gromacs/fileio
GMX= -I $(dir3) -L$(dir2) -lgromacs

all: performance test_a_star test_heap test_graph_implementation2 \
test_read_pdb test_read_parameters test_read_parameters_and_pdb

test_read_xtc: test_read_xtc.c $(P6)/read_xtc.c $(OBJS_D) $(A_STAR)
	$(CC) -o $@.out $^ $(GMX) $(FLAGS) $(FLAGS2) 

test_read_parameters_and_pdb: test_read_parameters_and_pdb.c $(OBJS_D)
	$(CC) -o $@.out $^ $(FLAGS) $(FLAGS2)
	./$@.out

test_read_parameters: test_read_parameters.c $(OBJS_C)
	$(CC) -o $@.out $^ $(FLAGS) $(FLAGS2)
	./$@.out

test_read_pdb: test_read_pdb.c $(OBJS_B)
	$(CC) -o $@.out $^ $(FLAGS) $(FLAGS2)
	./$@.out

performance: test_performance test_performance_zero test_performance2 \
test_performance_zero2

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
test_graph_implementation: test_graph_implementation.c \
../src/graph_implementation.c $(UNI)
	$(CC) test_graph_implementation.c  ../src/graph_implementation.c $(UNI) \
-o test_graph_implementation.out -I $(P)

just_a_test: test.c
	$(CC) test.c -o test.out
