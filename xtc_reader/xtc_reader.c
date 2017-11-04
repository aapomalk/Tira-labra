#include "/opt/gromacs-5.0.7/include/gromacs/fileio/xtcio.h"

#define FAIL 0
#define SUCCESS 1

int main(int argc, char ** argv) {
  t_fileio  *xd;
  char *filename;
  int natoms,step;
  real time,prec;
  matrix box;
  rvec *x;
  gmx_bool bOK;

  if (argc > 1) {
	filename=argv[1];
  }	else {
	printf("filename failed %s\n", argv[1]);
	return FAIL;
  }

  if ((xd = open_xtc(filename, "r")) == 0) {
	printf("open_xtc failed\n");
	return FAIL;
  }

  if (read_first_xtc(xd, &natoms, &step, &time, box, &x, &prec, &bOK) == 0) {
	printf("reading first frame failed\n");
	return FAIL;
  }
  printf("number of atoms: %d\n", natoms);
  
  return SUCCESS;
}
