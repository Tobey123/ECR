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
  job->source_code = "";
  job->command = "";
  job->has_source_code = false;

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
  job->command = strdup(cJSON_GetObjectItemCaseSensitive(json, "command")->valuestring);
  job->source_code = strdup(cJSON_GetObjectItemCaseSensitive(json, "source_code")->valuestring);
  job->has_source_code = cJSON_GetObjectItemCaseSensitive(json, "has_source_code")->valueint;

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
  cJSON *source_code = cJSON_CreateString(strdup(job->source_code));
  cJSON *command = cJSON_CreateString(strdup(job->command));   
  cJSON *has_source_code = cJSON_CreateBool(job->has_source_code);

  cJSON_AddItemToObject(json, "id", id);
  cJSON_AddItemToObject(json, "description", description);
  cJSON_AddItemToObject(json, "source_code", source_code);
  cJSON_AddItemToObject(json, "command", command);
  cJSON_AddItemToObject(json, "has_source_code", has_source_code);

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

