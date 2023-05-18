#ifdef AFL_SRC_INST
#ifndef MAIN_FN
#define MAIN_FN
#endif
#include "bufo.h"
static void initialize_shm(){
  char *data_shm_id_str = getenv(C_BUFO_SHM_ID);
  if(data_shm_id_str == NULL){
    fprintf(stderr, "Shm_id is not set properly.. aborting");
    abort();
  }
  sscanf(data_shm_id_str, "%d",&c_shm_id);
  cbd_ptr_u32 = shmat(c_shm_id,NULL,0);
  if (!cbd_ptr_u32) {
    fprintf(stderr,"shmat failed in lese.c");
    abort();
  }
  cbd_ptr = (cbd *)cbd_ptr_u32;
  cbd_ptr -> _c_Overflow_flag = 0;
  return;
}
#endif /* AFL_SRC_INST */

