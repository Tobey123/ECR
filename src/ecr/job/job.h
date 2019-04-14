#ifndef __ECR_JOB__
#define __ECR_JOB__

#include "../../cJSON/cJSON.h"

#define ECR_REDIS_JOB_PREFIX "ecr:job"

typedef struct _ecr_job {
  char *id;
  char *description;
  char *source_code;
  char *command;
  bool has_source_code;
} ecr_job;

ecr_job* ecr_job_new();
void ecr_job_destroy(ecr_job **job);
ecr_job* ecr_job_parse(char *data);
cJSON* ecr_job_tojson(ecr_job *job);
char* ecr_job_tostring(ecr_job *job);

#endif
