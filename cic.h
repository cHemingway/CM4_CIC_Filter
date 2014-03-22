/* 
 * Cascaded Integrator Comb filter implementation in C
 * API heavily inspired by CMSIS-DSP, will use same datatypes if included
 */


#ifndef CIC_H
#define CIC_H 

#include <stdint.h>

#ifndef CMSIS_DSP
typedef int16_t q15_t;
typedef int32_t q32_t;
#endif

struct cic_decimate_instance_q15 {
	uint16_t M;			/* Decimation Factor */
	uint8_t  N; 		/* Order */
	q32_t	 *pState;	/* State variable array */
};


int cic_decimate_init_q15(cic_decimate_instance_q15 *S, uint16_t M, uint8_t N, q32_t *pState, uint32_t blockSize);
							 
void cic_decimate_q15(const cic_decimate_instance_q15 *, q15_t *pSrc, q15_t *pDst, uint32_t blockSize);

/* CIC_H */
#endif 