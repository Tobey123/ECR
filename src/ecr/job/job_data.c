#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "job_data.h"

ecr_job_data* ecr_job_data_new() {
  ecr_job_data *job_data = (ecr_job_data*)malloc(sizeof(ecr_job_data));
  assert(job_data);
  job_data->content = "";
  job_data->is_command = true;
  job_data->lang = LANG_UNKNOWN;
  return job_data;
}

void ecr_job_data_destroy(ecr_job_data **job_data) {
  assert(job_data);
  if (*job_data) {
    ecr_job_data *self = *job_data;
    free (self);
    *job_data = NULL;
  }
}
