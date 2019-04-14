#ifndef __ECR_REDIS_CLIENT__
#define __ECR_REDIS_CLIENT__

#include <stdbool.h>
#include "../ecr/job/job.h"
#include "../ecr/status/status.h"

status_info* get_default_status_info();
ecr_job* get_default_job_info();
status_info* initRedis(char *hostname, int port, bool is_unix);
ecr_job* createJob(char *id, char *description, char *source_code, char *command, bool has_source_code);
status_info* storeJob(char *key, ecr_job *job);
ecr_job* retrieveJob(char *key);
status_info* removeJob(char *key);

#endif //__REDIS_CLIENT__
