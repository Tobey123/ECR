#ifndef __ECR_REDIS_CLIENT__
#define __ECR_REDIS_CLIENT__

#include <stdbool.h>
#include "../ecr/job/job.h"
#include "../ecr/status/status.h"

typedef status_info* (*_connect)(char *hostname, int port, bool is_unix);
typedef status_info* (*_disconnect)();
typedef ecr_job* (*_create_job)(char *id, char *description, ecr_job_data *data, bool is_command);
typedef ecr_job_data* (*_create_job_data)(char *content, bool is_command, language lang);
typedef status_info* (*_store_job)(ecr_job *job);
typedef ecr_job* (*_retrieve_job)(char *key);
typedef status_info* (*_remove_job)(char *key);

typedef struct _redis_client {
  _connect connect;
  _disconnect disconnect;
  _create_job create_job;
  _create_job_data create_job_data;
  _store_job store_job;
  _retrieve_job retrieve_job;
  _remove_job remove_job;
} redis_client;

redis_client* redis_client_new();
void redis_client_destroy(redis_client **client);

#endif //__REDIS_CLIENT__
