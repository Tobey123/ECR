#ifndef __ECR_JOB_DATA__
#define __ECR_JOB_DATA__

#include "../../cJSON/cJSON.h"

enum language_enum {
  MSDOS,
  POWERSHELL,
  JAVASCRIPT,
  PYTHON,
  C,
  CPP,
  LANG_UNKNOWN
};

typedef enum language_enum language;

typedef struct _ecr_job_data {
  char *content;
  bool is_command;
  language lang;
} ecr_job_data;

ecr_job_data* ecr_job_data_new(const char* content, bool is_command, language lang);
void ecr_job_data_destroy(ecr_job_data **job_data);
ecr_job_data* ecr_job_data_parse(char *job_data_str);
cJSON* ecr_job_data_tojson(ecr_job_data *job_data);
char* ecr_job_data_tostring(ecr_job_data *job_data);

#endif //__ECR_JOB_DATA__