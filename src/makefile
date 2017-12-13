CC= gcc -g -ansi -pedantic -Wall

PH=header_files
PG=graph_implementation
PA=a_star_implementation
PALLOC=allocation_implementation
PB=build_graph_implementation

GBV=$(PG)/graph.o $(PG)/box.o $(PG)/vertex.o
ALLOC=$(PALLOC)/allocation.o
A_STAR=$(PA)/a_star.o  $(PA)/heap.o
BUILD=$(PB)/read_pdb.o $(PB)/read_parameters.o $(PB)/read_xtc.o

ALL=$(GBV) $(ALLOC) $(A_STAR) $(BUILD)

DEPS=$(PH)/graph.h $(PH)/graph_implementation.h $(PH)/a_star.h \
$(PA)/heap.h $(PA)/a_star_implementation.h $(PH)/allocation.h \
$(PH)/build_graph_implementation.h $(PH)/build_graph.h \
$(PH)/pathfinder.h

dir2=/opt/gromacs-5.0.7/lib
dir3=/opt/gromacs-5.0.7/include/gromacs/fileio
GMX= -I $(dir3) -L$(dir2) -lgromacs

FLAGS= -I $(PH) -I $(PA) -lm $(GMX)

OBJS=pathfinder.o 

pathfinder: pathfinder_main.o $(ALL) $(OBJS)
	$(CC) -o $@.out $^ $(FLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(FLAGS)
