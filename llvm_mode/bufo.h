/*
 * bufo.h
 *
 *  Created on: Dec 8, 2016
 *      Author: 105880
 */

#ifndef BUFO_H_
#define BUFO_H_
#include <stdint.h> 
#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAIN_FN
#  define EXT_MAIN
#else
#  define EXT_MAIN extern
#endif /* ^MAIN_FN */

#define MAX_DATA_SIZE_POW  5
#define MAX_DATA_SIZE      (1 << MAX_DATA_SIZE_POW)
#define MAX_C_VAL          (1 << 7)
#define C_VAL_CHANGE_LIMIT 1  // This needs to be calibrated and checked.
#define C_BUFO_SHM_ID      "__C_BUFO_COMMAREA"

EXT_MAIN uint32_t* cbd_ptr_u32;         /* MRK - to record metric and ranges*/
EXT_MAIN int32_t c_shm_id;              /* MRK - ID of the our SHM region   */
EXT_MAIN uint32_t min_c_val;            /* MRK - Max of c_metric seen sofar */

struct c_buffo_data {                   /* Data to exchange                 */
	uint16_t  _cVal;                /* closeness metric                 */
	uint8_t   _c_Overflow_flag;     /* buffer overflow flag             */
	uint8_t   _c_aid;               /*  Ideally we need a vector        */
};                                      /* of ranges. Will do this later    */
typedef struct c_buffo_data cbd;
EXT_MAIN cbd *cbd_ptr;

#endif /* BUFO_H_ */

