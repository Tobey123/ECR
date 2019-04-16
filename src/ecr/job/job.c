#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "../base/helpers.h"
#include "job.h"

/**
 * @brief  Creates a new job instance
 * @note   
 * @retval Reference to job instance
 */
ecr_job* ecr_job_new() {
  ecr_job *job = (ecr_job*)malloc(sizeof(ecr_job));
  assert(job);

  job->id = "";
  job->description = "";
  job->data = (ecr_job_data*)malloc(sizeof(ecr_job_data));

  return job;
}
/**
 * @brief  Destroys a job instance
 * @note   
 * @param  **job: reference to job reference
 * @retval None
 */
void ecr_job_destroy(ecr_job **job) {
  assert(job);
  if (*job) {
    ecr_job *self = *job;
    free (self);
    *job = NULL;
  }
}
/**
 * @brief  Initializes a job by parsing it from JSON
 * @note   
 * @param  *job_str: JSON data
 * @retval reference to job instance
 */
ecr_job* ecr_job_parse(char *job_str) {
  assert(job_str);
  cJSON *job_json = cJSON_Parse(job_str);
  assert(job_json);

  ecr_job *job = ecr_job_new();
  job->id = strdup(cJSON_GetObjectItemCaseSensitive(job_json, "id")->valuestring);
  job->description = strdup(cJSON_GetObjectItemCaseSensitive(job_json, "description")->valuestring);
  // job->data = strdup(cJSON_GetObjectItemCaseSensitive(json, "data")->valuestring);

  cJSON *job_data = cJSON_GetObjectItem(job_json, "data");

  job->data->content = strdup(cJSON_GetObjectItemCaseSensitive(job_data, "content")->valuestring);
  job->data->is_command = (bool)(cJSON_GetObjectItemCaseSensitive(job_data, "is_command")->valueint);
  job->data->lang = (language)(cJSON_GetObjectItemCaseSensitive(job_data, "lang")->valueint);
  

  cJSON_Delete(job_json);
  cJSON_Delete(job_data);
  return job;
}
/**
 * @brief  Returns JSON description of a job
 * @note   
 * @param  *job: reference to job that should be returned as JSON
 * @retval cJSON instance containing JSON data
 */
cJSON* ecr_job_tojson(ecr_job *job) {
  assert(job);
  cJSON *json = cJSON_CreateObject();
  assert(json);

  cJSON *id = cJSON_CreateString(strdup(job->id));
  cJSON *description = cJSON_CreateString(strdup(job->description));

  cJSON *job_data = ecr_job_data_tojson(job->data);

  cJSON_AddItemToObject(json, "id", id);
  cJSON_AddItemToObject(json, "description", description);
  cJSON_AddItemToObject(json, "data", job_data);

  return json;
}
/**
 * @brief  Parses a job instance into a raw string
 * @note   
 * @param  *job: reference to job that should be parsed into raw string
 * @retval raw string containing job data
 */
char* ecr_job_tostring(ecr_job *job) {
  cJSON *json = ecr_job_tojson(job);
  char *as_string = strdup(cJSON_Print(json));
  free(json);
  return as_string;
}
