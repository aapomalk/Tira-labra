#include "gromacs/fileio/xtcio.h"

#define FAIL 0
#define SUCCESS 1

int main(int argc, char ** argv) {
  XDR *xd;
  char *filename;
  int natoms,step;
  real time,prec;
  matrix box;
  rvec *x;

  if (argc >= 1) {
	filename=argv[0];
  }	else {
	puts("filename failed %s\n", argv[0]);
	return FAIL;
  }

  if (open_xtc(xd, filename, "r") == 0) {
	puts("open_xtc failed\n");
	return FAIL;
  }

  
  return SUCCESS;
}
