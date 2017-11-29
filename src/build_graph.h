#ifndef BUILD_GRAPH_H
#define BUILD_GRAPH_H

/* 
   reads the files top, coord, and param
   -top is the topology, containing atoms and their indexes
   -param helps selecting the right atoms
   -coord contains the coordinates used for searching the path
   returns SUCCESS or FAIL
 */
int build_graph(char *top, char *coord, char* param, GRAPH *g);

/* used with trajectories */
int build_next_graph(GRAPH *g);

/* 
   Read a separate file containing several start-target index-pairs.
   Can be replaced with commandline interpreter like: -start 0 -target 100
   (if I manage to get the interpreter done).
 */
int build_path_starts_and_targets(char *start_targets, int *list, int *pairs);

#endif
