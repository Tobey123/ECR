#include <stdlib.h>
#include <assert.h>
#include "status.h"

status_info* status_info_new() {
  status_info *info = (status_info*)malloc(sizeof(status_info));
  
  info->code = REDIS_SUCCESS;
  info->message = "";
  
  return info;
}

void status_info_destroy(status_info **status) {
  assert(status);
  if (*status) {
    status_info *self = *status;
    free (self);
    *status = NULL;
  }
}
