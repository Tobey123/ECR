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

  job->id = "00";
  job->description = "";
  job->data = "";
  job->is_command = true;

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
 * @param  *data: JSON data
 * @retval reference to job instance
 */
ecr_job* ecr_job_parse(char *data) {
  assert(data);
  cJSON *json = cJSON_Parse(data);
  assert(json);

  ecr_job *job = ecr_job_new();
  job->id = strdup(cJSON_GetObjectItemCaseSensitive(json, "id")->valuestring);
  job->description = strdup(cJSON_GetObjectItemCaseSensitive(json, "description")->valuestring);
  job->data = strdup(cJSON_GetObjectItemCaseSensitive(json, "data")->valuestring);
  job->is_command = cJSON_GetObjectItemCaseSensitive(json, "is_command")->valueint;

  cJSON_Delete(json);
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
  cJSON *data = cJSON_CreateString(strdup(job->data));
  cJSON *is_command = cJSON_CreateBool(job->is_command);

  cJSON_AddItemToObject(json, "id", id);
  cJSON_AddItemToObject(json, "description", description);
  cJSON_AddItemToObject(json, "data", data);
  cJSON_AddItemToObject(json, "is_command", is_command);

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

