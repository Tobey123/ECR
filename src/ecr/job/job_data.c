#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "../base/helpers.h"
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

ecr_job_data* ecr_job_data_parse(char *job_data_str) {
  assert(job_data_str);
  cJSON *job_data_json = cJSON_Parse(job_data_str);
  assert(job_data_json);

  ecr_job_data *job_data = ecr_job_data_new();
  job_data->content = strdup(cJSON_GetObjectItemCaseSensitive(job_data_json, "content")->valuestring);
  job_data->is_command = (bool)(cJSON_GetObjectItemCaseSensitive(job_data_json, "is_command")->valueint);
  job_data->lang = (language)(cJSON_GetObjectItemCaseSensitive(job_data_json, "lang")->valueint);

  cJSON_Delete(job_data_json);
  return job_data;
}

cJSON* ecr_job_data_tojson(ecr_job_data *job_data) {
  assert(job_data);
  cJSON *job_data_json = cJSON_CreateObject();
  assert(job_data_json);

  cJSON *content = cJSON_CreateString(strdup(job_data->content));
  cJSON *is_command = cJSON_CreateBool(job_data->is_command);
  cJSON *lang = cJSON_CreateNumber((int)job_data->lang);

  cJSON_AddItemToObject(job_data_json, "content", content);
  cJSON_AddItemToObject(job_data_json, "is_command", is_command);
  cJSON_AddItemToObject(job_data_json, "lang", lang);

  return job_data_json;
}
char* ecr_job_data_tostring(ecr_job_data *job_data) {
  cJSON *json = ecr_job_data_tojson(job_data);
  char *as_string = strdup(cJSON_Print(json));
  free(json);
  return as_string;
}