#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "job.h"

/**
 * @brief  Creates a job instance
 * @note   
 * @param  *id: job id
 * @param  *description: job bescription 
 * @param  *job_data: Pointer to job data
 * @retval Pointer to job instance
 */
ecr_job* ecr_job_new(const char *id, const char *description, ecr_job_data *job_data) {
  ecr_job *job = (ecr_job*)malloc(sizeof(ecr_job));
  assert(job);

  job->id = strndup(id, strlen(id));
  job->description = strndup(description, strlen(description));
  job->data = job_data;

  return job;
}
/**
 * @brief  Destroys a job instance
 * @note   
 * @param  **job: Pointer to job pointer
 * @retval None
 */
void ecr_job_destroy(ecr_job **job) {
  assert(job);
  if (*job) {
    ecr_job *self = *job;
    ecr_job_data_destroy(&self->data);
    free (self);
    *job = NULL;
  }
}
/**
 * @brief  Initializes a job by parsing it from JSON
 * @note   
 * @param  *job_str: JSON data
 * @retval Pointer to job instance
 */
ecr_job* ecr_job_parse(char *job_str) {
  assert(job_str);
  cJSON *job_json = cJSON_Parse(job_str);
  assert(job_json);

  char *_id = cJSON_GetObjectItemCaseSensitive(job_json, "id")->valuestring;
  char *_description = cJSON_GetObjectItemCaseSensitive(job_json, "description")->valuestring;
  
  char *id = strndup(_id, strlen(_id));
  char *description = strndup(_description, strlen(_description));

  cJSON *job_data_json = cJSON_GetObjectItem(job_json, "data");

  char *_content = cJSON_GetObjectItemCaseSensitive(job_data_json, "content")->valuestring;
  char *content = strndup(_content, strlen(_content));

  ecr_job_data *job_data = ecr_job_data_new(content,
                          (bool)(cJSON_GetObjectItemCaseSensitive(job_data_json, "is_command")->valueint),
                          (language)(cJSON_GetObjectItemCaseSensitive(job_data_json, "lang")->valueint)
  );

  ecr_job *job = ecr_job_new(id, description, job_data);
  
  cJSON_Delete(job_json);
  cJSON_Delete(job_data_json);
  return job;
}
/**
 * @brief  Returns JSON description of a job
 * @note   
 * @param  *job: reference to job that should be returned as JSON
 * @retval cJSON pointer
 */
cJSON* ecr_job_tojson(ecr_job *job) {
  assert(job);
  cJSON *json = cJSON_CreateObject();
  assert(json);

  cJSON *id = cJSON_CreateString(strndup(job->id, strlen(job->id)));
  cJSON *description = cJSON_CreateString(strndup(job->description, strlen(job->description)));

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
  char *dump = cJSON_Print(json);
  char *as_string = strndup(dump, strlen(dump));
  free(json);
  return as_string;
}
