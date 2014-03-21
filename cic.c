#include <cic.h>


int cic_decimate_init_q15(cic_decimate_instance_q15 *S, uint16_t M, uint8_t N, q15_t *pState, uint32_t blockSize) {

	if((S==NULL) || (pState==NULL)) { /* Check for null pointers */
		return -1;
	}


	return 0; /* Success */
}


void cic_decimate_q15(const cic_decimate_instance_q15 *S, q15_t *pSrc, q15_t *pDst, uint32_t blockSize)
{


	
}