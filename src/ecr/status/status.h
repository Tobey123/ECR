#ifndef __ECR_STATUS__
#define __ECR_STATUS__

enum redis_return_value {
  REDIS_SUCCESS, 
  REDIS_ERROR,
  REDIS_FAILED,
  REDIS_UNKNOWN
};

typedef enum redis_return_value redis_status_code;

typedef struct _status_info {
  redis_status_code code;
  char *message;
} status_info;

status_info* status_info_new();
void status_info_destroy(status_info **status);

#endif
