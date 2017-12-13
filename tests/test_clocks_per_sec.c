#include <time.h>
#include <stdio.h>
#include <stdlib.h>

/* the sole purpose of this program is to print CLOCKS_PER_SEC constant */
int main() {
  printf("clocks per sec %ld\n", CLOCKS_PER_SEC);
  return 0;
}
