#include "xtcio.h"

#define FAIL 0
#define SUCCESS 1

int main(int argc, char ** argv) {
  t_fileio  *xd;
  char *filename;
  int natoms,step,i,j;
  real time,prec;
  matrix box;
  rvec *x;
  gmx_bool bOK;

  if (argc > 1) {
	filename=argv[1];
  }	else {
	printf("filename failed\nprogram needs .xtc -filename as parameter\nLike this %s test.xtc\n", argv[0]);
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
  printf("natoms: %d, step: %d, time: %.2f, prec: %.2f, bOK: %d\n", natoms, step, time, prec, bOK);
  printf("box: ");
  for (i=0; i<3; i++) {
	for (j=0; j<3; j++) {
	  printf("%.3f%s", box[i][j], (i == 2 && j == 2) ? "\n" : ", ");
	}
  }
  printf("coordinates:\n");
  for (i=0; i<natoms; i++) {
	printf("%d: ", i);
	for (j=0; j<3; j++) { /* If j==3 then it's the same as if j==0 and i +1 larger */
	  printf("%.3f%s", x[i][j], (j == 2) ? "" : " ");
	}
	printf("\n");
  }
  
  return SUCCESS;
}
