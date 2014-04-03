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

	/* COMB and DECIMATE */
	gain = M; /* As only 1st order */
	pState[1] = pState[1] - pState[blockSize-M]; /* Wrap around from last block */
	pDst[0] = pState[1] / gain;
	for (i=M, j=1; i<blockSize; i+=M, j++) {
		pDst[j] = (pState[i] - pState[i-M]) / gain ;
	}
}

int cic_decimate_init_q32(cic_decimate_instance_q32 *S, uint16_t M, uint8_t N, uint8_t R, q32_t *pState1, q32_t *pState2, uint32_t blockSize)
{
	if((S==NULL) || (pState1==NULL) || (pState2==NULL)) { /* Check for null pointers */
		return -1;
	}

	if (blockSize%M != 0) { /* Block size is not a multiple of M */
		return -1;
	}

	S->M = M;
	S->N = N;
	S->R = R;
	S->pState1 = pState1;
	S->pState2 = pState2;
	S->nSample = 0;
	return 0; /* Success */
}

void cic_decimate_q32(cic_decimate_instance_q32 *S, q32_t *pSrc, q32_t *pDst, uint32_t blockSize)
{
	int i = 0, j = 0;
	q32_t *pInt =  	S->pState1; 	/*State after integrator */
	q32_t *pCombIn = S->pState2;		/*State after decimator */
	uint16_t M = 	S->M; 			/*Decimation Factor */
	uint8_t	 R = 	S->R;			/*Differential delay scale in comb */
	uint8_t	 nStages = 	S->N;		/*Number of stages */
	int nSample = S->nSample;


	/* Downsample each element one by one */
	j = 0; /* Output Index */
	for (i=0; i<blockSize; i++) {
		int n;

		/*Integrate*/
		pInt[0] = pSrc[i];
		for (n=1; n<nStages+1; n++) {
			pInt[n] = pInt[n]+ pInt[n-1]; 
		}

		if (nSample==M) { /*If we are at a decimate block*/
			pCombIn[0] = pInt[nStages]; /*Copy output into Comb */

			/* Comb */
			for(n=1; n<nStages; n++) {
				pCombIn[n+1] = pCombIn[n] - pCombIn[n-1];
			}
			/* Output */
			pDst[j] = pCombIn[nStages];
			j++;
			nSample = 0;
		}
		else {
			nSample += 1;
		}

	}
	S->nSample = nSample;
}