#ifdef _WIN32
        #include <windows.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis/hiredis.h>
#include "redis_client.h"

struct timeval timeout = { 1, 500000 }; // 1.5 seconds
redisContext *ctx; // global & private

status_info* prv_connect(char *hostname, int port, bool is_unix);
status_info* prv_disconnect();
status_info* prv_store_job(ecr_job *job);
ecr_job* prv_retrieve_job(char *key);
status_info* prv_remove_job(char *key);
ecr_job* prv_create_job(char *id, char *description, char *source_code, char *command, bool has_source_code);

/**
 * @brief  Initializes a redis client
 * @note   
 * @retval reference to redis_client struct
 */
redis_client* redis_client_new() {
  redis_client *client = (redis_client*)malloc(sizeof(redis_client));
  assert(client);
  client->connect = prv_connect;
  client->disconnect = prv_disconnect;
  client->create_job = prv_create_job;
  client->remove_job = prv_remove_job;
  client->retrieve_job = prv_retrieve_job;
  client->store_job = prv_store_job;
  return client;
}
/**
 * @brief  Destroys a redis client
 * @note   
 * @param  **client: 
 * @retval None
 */
void redis_client_destroy(redis_client **client) {
  assert(client);
  if (*client) {
    redis_client *self = *client;
    free (self);
    *client = NULL;
  }
}
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
    info->message = strdup(reply->str);
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
  ecr_job *job = ecr_job_new();

  redisReply *reply = redisCommand(ctx, "GET %s:%s", ECR_REDIS_JOB_PREFIX, key);
  assert(reply);

  cJSON *json = cJSON_Parse(reply->str);
  assert(json);

  job->id = strdup(cJSON_GetObjectItemCaseSensitive(json, "id")->valuestring);
  job->description = strdup(cJSON_GetObjectItemCaseSensitive(json, "description")->valuestring);
  job->command = strdup(cJSON_GetObjectItemCaseSensitive(json, "command")->valuestring);
  job->source_code = strdup(cJSON_GetObjectItemCaseSensitive(json, "source_code")->valuestring);
  job->has_source_code = cJSON_GetObjectItemCaseSensitive(json, "has_source_code")->valueint;

  freeReplyObject(reply);
  cJSON_Delete(json);
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
  info->message = strdup(reply->str);

  return info;
}
/**
 * @brief  Creates a new job
 * @note   
 * @param  *id: job id
 * @param  *description: job description 
 * @param  *source_code: source code
 * @param  *command: command -> will be ignored if has_source_code is true
 * @param  has_source_code: does this job carry source_code
 * @retval reference to job instance
 */
ecr_job* prv_create_job(char *id, char *description, char *source_code, char *command, bool has_source_code) {
  assert(id);
  assert(description);
  assert(source_code);
  assert(command);
  ecr_job *job = ecr_job_new();
  assert(job);

  job->id = strdup(id);
  job->description = strdup(description);
  job->source_code = strdup(source_code);
  job->command = strdup(command);
  job->has_source_code = has_source_code;

  return job;
}
