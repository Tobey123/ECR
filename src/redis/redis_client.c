#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis/hiredis.h>
#include "redis_client.h"

struct timeval timeout = { 1, 500000 }; // 1.5 seconds
redisContext *ctx; // global & private

status_info* initRedis(char *hostname, int port, bool is_unix) {
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
            status->code = OK;
            sprintf(status->message, "Connection error: %s\n", ctx->errstr);
        } else {
            status->code = OK;
            sprintf(status->message, "Connection error: can't allocate redis context\n");
        }
        return status;
    }
    status->code = OK,
    status->message = "SUCCESS";
    return status;
}

status_info* storeJob(char *key, ecr_job *job) {
    assert(key);
    assert(job);
    char *job_string = ecr_job_tostring(job);
    status_info *info = status_info_new();
    assert(ctx);

    redisReply *reply = redisCommand(ctx, "SET %s:%b %b", ECR_REDIS_JOB_PREFIX, key, strlen(key), job_string, strlen(job_string));
    assert(reply);

    info->code = OK;
    info->message = strdup(reply->str);

    info->code = OK;
    freeReplyObject(reply);

    return info;
}

ecr_job* retrieveJob(char *key) {
  assert(key);
  ecr_job *job = ecr_job_new();
  assert(ctx);

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

status_info* removeJob(char *key) {
  status_info *info = status_info_new();
  assert(info);

  info->code = OK;
  info->message = "job data here";

  return info;
}

ecr_job* createJob(char *id, char *description, char *source_code, char *command, bool has_source_code) {
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
