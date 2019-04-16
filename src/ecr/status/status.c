#include <stdlib.h>
#include <assert.h>
#include "status.h"

/**
 * @brief  Creates a new status_info object
 * @note   
 * @retval reference to status_info reference
 */
status_info* status_info_new() {
  status_info *info = (status_info*)malloc(sizeof(status_info));
  
  info->code = REDIS_STATUS_SUCCESS;
  info->message = "";
  
  return info;
}

/**
 * @brief  Destroys a status_info reference
 * @note   
 * @param  **status: status_info reference to be destroyed
 * @retval None
 */
void status_info_destroy(status_info **status) {
  assert(status);
  if (*status) {
    status_info *self = *status;
    free (self);
    *status = NULL;
  }
}
