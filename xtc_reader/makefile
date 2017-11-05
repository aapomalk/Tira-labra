CC= gcc -ansi -pedantic -Wall
#dir=/opt/gromacs-5.0.7/include/gromacs/fileio
dir=/home/work/aapomalk/Downloads/automated_install/gromacs-5.0.7/src/gromacs/fileio
dir2=/opt/gromacs-5.0.7/lib
dir3=/opt/gromacs-5.0.7/include/gromacs/fileio
#GMX= -lgmx.$(CPU)
GMX= -I $(dir3) -L$(dir2) -lgromacs
#GMX= -L$(dir)

xtc_reader: xtc_reader.c
	$(CC) -o xtc_reader.out xtc_reader.c $(GMX)
