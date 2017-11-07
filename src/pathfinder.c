#include "pathfinder.h"
#include "graph.h"
#include "build_graph.h"
#include "constants.h"
#include <stdio.h>

#define NUMBER_OF_FILES 3

int run_pathfinder(int argc, char ** argv) {
	GRAPH *g;
	char *top, *coord, *param; /* filenames */
	if (argc <= NUMBER_OF_FILES) {
		printf("program needs at least 3 filenames, for example:\n");
		printf("%s system.pdb system.xtc parameters.txt\n", argv[0]);
		return FAIL;
	}
	
	
	if (build_graph(top, coord, param, g) == FAIL) {
		return FAIL;
	}
	
	return SUCCESS;
}