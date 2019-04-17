#ifdef _WIN32
        #include <windows.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis/hiredis.h>
#include "redis_client_private.h"
#include "../ecr/base/helpers.c"

static struct timeval timeout = { 1, 500000 }; // 1.5 seconds
static redisContext *ctx; // global & private

static status_info* prv_connect(char *hostname, int port, bool is_unix);
static status_info* prv_disconnect();
static status_info* prv_store_job(ecr_job *job);
static ecr_job* prv_retrieve_job(char *key);
static status_info* prv_remove_job(char *key);
static ecr_job* prv_create_job(char *id, char *description, ecr_job_data *data);
static ecr_job_data* prv_create_job_data(char *content, bool is_command, language lang);

/**
 * @brief  Opens a connection to a redis instance
 * @note   
 * @param  *hostname: Server where redis is running
 * @param  port: Server port
 * @param  is_unix: Is client OS a Unix system
 * @retval status_info reference containing return code
 */
status_info* prv_connect(char *hostname, int port, bool is_unix) {
  assert(!ctx);
  status_info *status = status_info_new();
  assert(status);

  if (is_unix) {
        ctx = redisConnectUnixWithTimeout(hostname, timeout);
    } else {
        ctx = redisConnectWithTimeout(hostname, port, timeout);
    }
    if (ctx == NULL || ctx->err) {
        if (ctx) {
            redisFree(ctx);
            status->code = REDIS_STATUS_SUCCESS;
            sprintf(status->message, "Connection error: %s\n", ctx->errstr);
        } else {
            status->code = REDIS_STATUS_SUCCESS;
            sprintf(status->message, "Connection error: can't allocate redis context\n");
        }
        return status;
    }
    status->code = REDIS_STATUS_SUCCESS,
    status->message = "SUCCESS";
    return status;
}
/**
 * @brief  Disconnects from redis server
 * @note   
 * @retval status_info reference containing return code
 */
status_info* prv_disconnect() {
  assert(ctx);
  status_info *info = status_info_new();
  redisFree(ctx);
  info->code = REDIS_STATUS_SUCCESS;
  info->message = "Connection to redis closed";
  return info;
}
/**
 * @brief  Stores a job in redis db
 * @note   
 * @param  *job: reference to job
 * @retval status_info reference containing return code
 */
status_info* prv_store_job(ecr_job *job) {
    assert(ctx);
    assert(job);
    char *job_string = ecr_job_tostring(job);
    status_info *info = status_info_new();

    redisReply *reply = redisCommand(ctx, "SET %s:%b %b", ECR_REDIS_JOB_PREFIX, job->id, strlen(job->id), job_string, strlen(job_string));
    assert(reply);

    info->code = REDIS_STATUS_SUCCESS;
    info->message = _strdup(reply->str);
    freeReplyObject(reply);
    return info;
}
/**
 * @brief  Retrieves a job from redis db
 * @note   
 * @param  *key: job id
 * @retval reference to job instance 
 */
ecr_job* prv_retrieve_job(char *key) {
  assert(ctx);
  assert(key);

  redisReply *reply = redisCommand(ctx, "GET %s:%s", ECR_REDIS_JOB_PREFIX, key);
  assert(reply);

  cJSON *job_json = cJSON_Parse(reply->str);
  assert(job_json);

  char *id = _strdup(cJSON_GetObjectItemCaseSensitive(job_json, "id")->valuestring);
  char *description = _strdup(cJSON_GetObjectItemCaseSensitive(job_json, "description")->valuestring);

  cJSON *job_data_json = cJSON_GetObjectItem(job_json, "data");

  ecr_job_data *job_data = ecr_job_data_new(_strdup(cJSON_GetObjectItemCaseSensitive(job_data_json, "content")->valuestring),
                   (bool)cJSON_GetObjectItemCaseSensitive(job_data_json, "is_command")->valueint,
                   (language)cJSON_GetObjectItemCaseSensitive(job_data_json, "lang")->valueint
  );

  ecr_job *job = ecr_job_new(id, description, job_data);


  freeReplyObject(reply);
  cJSON_Delete(job_json);
  return job;
}
/**
 * @brief  Removes a job from redis db
 * @note   
 * @param  *key: job id
 * @retval status_info reference containing return code
 */
status_info* prv_remove_job(char *key) {
  assert(ctx);
  assert(key);
  status_info *info = status_info_new();
  assert(info);

  redisReply *reply = redisCommand(ctx, "DEL %s:%s", ECR_REDIS_JOB_PREFIX, key);
  assert(reply);

  info->code = REDIS_STATUS_SUCCESS;
  info->message = _strdup(reply->str);

  return info;
}
/**
 * @brief  Creates a new job
 * @note   
 * @param  *id: job id
 * @param  *description: job description 
 * @param  *data: Pointer to job_data instance
 * @retval reference to job instance
 */
ecr_job* prv_create_job(char *id, char *description, ecr_job_data *data) {
  assert(id);
  assert(description);
  assert(data);
  ecr_job *job = ecr_job_new(_strdup(id),
                            _strdup(description),
                            data
  );
  return job;
}
/**
 * @brief  Create job_data instance
 * @note   
 * @param  *content: String content of command or script
 * @param  is_command: Is the content a command
 * @param  lang: Language used to parse script or command
 * @retval 
 */
ecr_job_data* prv_create_job_data(char *content, bool is_command, language lang) {
  assert(content);
  ecr_job_data *job_data = ecr_job_data_new(_strdup(content), is_command, lang);
  return job_data;
}
