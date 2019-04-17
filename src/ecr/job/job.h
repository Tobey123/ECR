#ifndef __ECR_JOB__
#define __ECR_JOB__

#include "../../cJSON/cJSON.h"
#include "job_data.h"

#define ECR_REDIS_JOB_PREFIX "ecr:job"

typedef struct _ecr_job {
  char *id;
  char *description;
  ecr_job_data *data;
} ecr_job;

ecr_job* ecr_job_new(const char *id, const char *description, ecr_job_data *job_data);
void ecr_job_destroy(ecr_job **job);
ecr_job* ecr_job_parse(char *job_str);
cJSON* ecr_job_tojson(ecr_job *job);
char* ecr_job_tostring(ecr_job *job);

#endif
