#include <stdio.h>
#include <stdlib.h>
#include "cic.h"

void print_usage(const char name[]) {
	printf(
	"%s: Performs a CIC filter on matlab vector \n"
	"Usage: %s in [m] [out] \n"
	"\tin:\t1 value per line 16 bit input data \n"
	"\tm:\tdecimation factor, 1000 if not set \n"
	"\tout: \t1 value per line 16 bit output data, out.txt if not set \n",
	name, name);
}


int main(int argc, char const *argv[])
{
	char default_outfilename[] = "out.txt";
	const char *infilename = NULL, *outfilename = default_outfilename;
	int M = 1000;

	/* Check for correct number of command line arguments */
	if ((argc<2) || (argc>4)) {
		print_usage(argv[0]);
		return -1;
	}

	/* Parse Arguments */
	if (argc >= 2) { /* infile specified */
		infilename = argv[1];
	}
	if (argc >= 3) { /* infile and M specified */
		M = atoi(argv[2]);
		if (M==0) { /* M is not a number */
			print_usage(argv[0]);
			return -1;
		}
	}
	if (argc >= 4) {
		outfilename = argv[3];
	} 
	


	return 0; /* Success */
}