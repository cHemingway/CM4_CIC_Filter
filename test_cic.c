#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include "cic.h"

#define LINE_TERMINATOR "\n"

void print_file_error(const char filename[]) {
	printf("Error opening file %s: %s \n", 
		filename, strerror(errno));
}

void print_oom_error(void) {
	printf("Error memory allocation failed %s",strerror(errno));
}

void print_usage(const char name[]) {
	printf(
	"%s: Performs a CIC filter on matlab vector \n"
	"Usage: %s in [m] [out] \n"
	"\tin:\t1 value per line 16 bit input data, must be larger than the decimation factor \n"
	"\tm:\tdecimation factor, 1000 if not set \n"
	"\tout: \t1 value per line 16 bit output data, out.txt if not set \n",
	name, name);
}


/* Reads a integer value, 1 value per line. Returns -1 on error */
int read_input(FILE *infile, q15_t *out) {
	char line[10]; /* 2^16 = 65536, include some room for leading whitespace */

	if (NULL!=fgets(line, sizeof(line), infile) ) {
		*out = atoi(line);
		return 0;  /* Success */
	}
	else {
		return -1; /* Error in fgets */
	}
}

int main(int argc, char const *argv[])
{
	char default_outfilename[] = "out.txt";
	const char *infilename = NULL, *outfilename = default_outfilename;
	FILE *infile, *outfile;
	int M = 1000;
	q15_t *indata, *outdata;
	q15_t indatum; /* Single data point */
	q32_t *pState;
	int indatasize, outdatasize; /* Size is in number of q15, not bytes */
	int linesin, linesout, blocksize, i;
	cic_decimate_instance_q15 decimate_instance;

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

	/* Attempt to open input file */
	infile = fopen(infilename, "r");
	if (infile==NULL) {
		print_file_error(infilename);
		return -1;
	}

	/* Attempt to open output file */
	outfile = fopen(outfilename, "w");
		if (outfile==NULL) {
		print_file_error(outfilename);
		return -1;
	}

	/* Allocate initial space for input data */
	indatasize = 100 * M; /* 100X decimation factor seems a good bet */
	indata = calloc(indatasize, sizeof(indata[0]));
	if (indata==NULL) { /* Check if we run out of memory */
		print_oom_error();
		return -1;
	}
	
	/* Read all input data in */
	linesin = 0; /* Index of input data */
	while(-1!=read_input(infile, &indatum)) {
		indata[linesin++] = indatum;
		if (linesin==indatasize) { /* Expand input array by M*/
			indatasize += 100 * M;
			indata = realloc(indata, indatasize * sizeof(indata[0]));
			if (indata==NULL) { /* Check if we run out of memory */
				print_oom_error();
				return -1;
			}
		}
	}

	/* Check input is the right size */
	if (linesin<M) {
		printf("Error input size is less than decimation factor \n");
		return -1;
	}

	/* Allocate initial space for output data */
	outdatasize = (indatasize / M) + M;
	outdata = calloc(outdatasize, sizeof(outdata[0]));
	if (outdata==NULL) { /* Check if we run out of memory */
		print_oom_error();
		return -1;
	}

	/* DECIMATE */
	blocksize = (linesin/M) * M; /* Do it all in the largest possible block for now */
	pState = calloc(blocksize, sizeof(outdata));
	assert(0==cic_decimate_init_q15(&decimate_instance, M, 1, pState, blocksize));
	cic_decimate_q15(&decimate_instance, indata, outdata, blocksize);


	/* Output data */
	linesout = linesin / M;
	for (i=0; i<linesout; i++) {
		fprintf(outfile, "%d" LINE_TERMINATOR, outdata[i]);
	}

	return 0; /* Success */
}