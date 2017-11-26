#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
  int i=0;
  printf("this is just a test\n");
  do {
	printf("testing %d\n", i);
	if (i > 1000) {
	  break;
	}
	i++;
  } while (10 >= 0 && 10 != 128 && 10 < 500);
  return 1;
}
