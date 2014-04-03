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
	"Usage: %s in [m] [type] [n] [r] [out] \n"
	"\tin:\t1 value per line 16 bit input data, must be larger than m\n"
	"\tm:\tdecimation factor, 1000 if not set \n"
	"\ttype:\ttype of decimator, 32 or 16. 32 if not set \n"
	"\tn:\tnumber of stages, 3 if not set \n"
	"\tr:\tdifferential delay (normally 1 or 2), 2 if not set \n"
	"\tout: \t1 value per line 16 bit output data, out.txt if not set \n",
	name, name);
}


/* Reads a integer value, 1 value per line. Returns -1 on error */
int read_input(FILE *infile, int *out) {
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
	int M = 1000, N = 3, R = 2;
	int type = 16; /* Default of 16 bit */
	q15_t *indata_q15, *outdata_q15;
	q32_t *indata_q32, *outdata_q32;
	int indatum; /* Single data point */
	q32_t *pState, *pState1, *pState2;
	int indatasize, outdatasize; /* Size is in number of q15, not bytes */
	int linesin, linesout, blocksize, i;
	cic_decimate_instance_q15 dec_instance_q15;
	cic_decimate_instance_q32 dec_instance_q32;

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
	if (argc >= 4) { /* infile and M and type specified */
		type = atoi(argv[3]);
		if ((type!=16) || (type!=32)) {
			printf("Invalid type specified \n");
			print_usage(argv[0]);
			return -1;
		}
	}
	if (argc >= 5) { /* infile and M and type and N specified */
		N = atoi(argv[4]);
		if (N<1) {
			printf("Incorrect value (<1) for N \n");
			print_usage(argv[0]);
			return -1;
		}
	}
	if (argc >= 6) { /* infile and M and type and N and R specified */
		R = atoi(argv[5]);
		if (R<1) {
			printf("Incorrect value (<1) for R \n");
			print_usage(argv[0]);
			return -1;
		}
	}
	if (argc >= 7) { /* All arguments specified */
		outfilename = argv[6];
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
	if (type == 16) {
		indata_q15 = calloc(indatasize, sizeof(indata_q15[0]));
		if (indata_q15==NULL) { /* Check if we run out of memory */
			print_oom_error();
			return -1;
		}
	}
	else if (type == 32) {
		indata_q32 = calloc(indatasize, sizeof(indata_q32[0]));
		if (indata_q32==NULL) { /* Check if we run out of memory */
			print_oom_error();
			return -1;
		}
	}
	
	/* Read all input data in */
	linesin = 0; /* Index of input data */
	while(-1!=read_input(infile, &indatum)) {
		if (type == 16) {
			indata_q15[linesin++] = indatum;
			if (linesin==indatasize) { /* Expand input array by M*/
				indatasize += 100 * M;
				indata_q15 = realloc(indata_q15, indatasize * sizeof(indata_q15[0]));
				if (indata_q15==NULL) { /* Check if we run out of memory */
					print_oom_error();
					return -1;
				}
			}
		} else if (type == 32) {
			indata_q32[linesin++] = indatum;
			if (linesin==indatasize) { /* Expand input array by M*/
				indatasize += 100 * M;
				indata_q32 = realloc(indata_q32, indatasize * sizeof(indata_q32[0]));
				if (indata_q32==NULL) { /* Check if we run out of memory */
					print_oom_error();
					return -1;
				}
			}

		}
	}

	/* Check input is the right size */
	if (linesin<M) {
		printf("Error input size is less than decimation factor \n");
		return -1;
	}

	/* Allocate initial space for output data */
	if (type==16) {
		outdatasize = (indatasize / M) + M;
		outdata_q15 = calloc(outdatasize, sizeof(outdata_q15[0]));
		if (outdata_q15==NULL) { /* Check if we run out of memory */
			print_oom_error();
			return -1;
		}
	}
	else if (type==32) {
		outdatasize = (indatasize / M) + M;
		outdata_q32 = calloc(outdatasize, sizeof(outdata_q32[0]));
		if (outdata_q32==NULL) { /* Check if we run out of memory */
			print_oom_error();
			return -1;
		}
	}

	/* DECIMATE */
	if (type==16) {
		blocksize = (indatasize/M)*M; 
		pState = calloc(blocksize, sizeof(outdata_q15[0]));
		assert(0==cic_decimate_init_q15(&dec_instance_q15, M, 1, pState, blocksize));
		cic_decimate_q15(&dec_instance_q15, indata_q15, outdata_q15, blocksize);
	}
	else if (type==32) {
		blocksize = (indatasize/M)*M; 
		pState1 = calloc(blocksize, sizeof(outdata_q32[0]));
		pState2 = calloc(blocksize, sizeof(outdata_q32[0]));
		assert(0==cic_decimate_init_q32(&dec_instance_q32, M, N, R, pState1, pState2, blocksize));
		cic_decimate_q32(&dec_instance_q32, indata_q32, outdata_q32, blocksize);
	}


	/* Output data */
	linesout = linesin / M;
	if (type == 16) {
		for (i=0; i<linesout; i++) {
			fprintf(outfile, "%d" LINE_TERMINATOR, outdata_q15[i]);
		}
	} else {
		for (i=0; i<linesout; i++) {
			fprintf(outfile, "%d" LINE_TERMINATOR, outdata_q32[i]);
		}
	}

	return 0; /* Success */
}