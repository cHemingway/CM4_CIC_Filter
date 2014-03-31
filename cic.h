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

typedef struct cic_decimate_instance_q15_t {
	uint16_t M;			/* Decimation Factor */
	uint8_t  N; 		/* Order */
	q32_t	 *pState;	/* State variable array */
} cic_decimate_instance_q15;

typedef struct cic_decimate_instance_q32_t {
	uint16_t M;			/* Decimation Factor */
	uint8_t  N; 		/* Order */
	uint8_t	 R;			/* Delay scale of comb filter */
	uint32_t G;			/* DC Gain */
	q32_t	 *pState1;	/* State variable array */
	q32_t	 *pState2;
} cic_decimate_instance_q32;


int cic_decimate_init_q15(cic_decimate_instance_q15 *S, uint16_t M, uint8_t N, q32_t *pState, uint32_t blockSize);
							 
void cic_decimate_q15(const cic_decimate_instance_q15 *S, q15_t *pSrc, q15_t *pDst, uint32_t blockSize);

int cic_decimate_init_q32(cic_decimate_instance_q32 *S, uint16_t M, uint8_t N, uint8_t R, q32_t *pState1, q32_t *pState2, uint32_t blockSize);

void cic_decimate_q32(const cic_decimate_instance_q32 *S, q32_t *pSrc, q32_t *pDst, uint32_t blockSize);
/* CIC_H */
#endif 