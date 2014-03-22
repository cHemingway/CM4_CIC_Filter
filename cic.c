#include "cic.h"
#include <stddef.h>


int cic_decimate_init_q15(cic_decimate_instance_q15 *S, uint16_t M, uint8_t N, q32_t *pState, uint32_t blockSize) {

	if((S==NULL) || (pState==NULL)) { /* Check for null pointers */
		return -1;
	}

	if (blockSize%M != 0) { /* Block size is not a multiple of M */
		return -1;
	}

	S->M = M;
	S->N = N;
	S->pState = pState;
	return 0; /* Success */
}


/* First order only for now */
void cic_decimate_q15(const cic_decimate_instance_q15 *S, q15_t *pSrc, q15_t *pDst, uint32_t blockSize)
{
	int i, j = 0;
	q32_t *pState = S->pState; 	/*State */
	uint16_t M = S->M; 			/*Decimation Factor */
	uint16_t gain;				/* DC Gain */

	/* INTEGRATE */
	pState[0] = pState[blockSize-1] + pSrc[0]; 	/* Wrap around from last block */
	for (i=1; i<blockSize; i++) {
		pState[i] = pState[i-1] + pSrc[i];
	}
	/* COMB */
	pState[0] = pState[0] - pState[(blockSize-1)-M]; /* Wrap around from last block */
	for (i=M; i<blockSize; i+=M) {
		pState[i] = pState[i] - pState[i-M];
	}

	/* DECIMATE */
	gain = M; /* As only 1st order */
	/* i = input index, j = output index */
	for (i=0, j=0; i<blockSize; i+=M, j++) {
		pDst[j] = pState[i] / gain; /* Remove DC Gain */
	}
}