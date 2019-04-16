/*
 * @CreateTime: Apr 14, 2019 9:46 PM
 * @Author: Harris Brakmic
 * @Contact: brakmic@gmail.com
 * @Last Modified By: brakmic
 * @Last Modified Time: Apr 14, 2019 9:47 PM
 * @Description: Execute Collect Report -  A tool for remote code execution
 */


#include "src/ecr/base/base.h"

int main(int argc, char **argv) {

  status_info *redis_info = initRedis("localhost", 6379, false);

  if (redis_info->code == REDIS_STATUS_SUCCESS) {
    ecr_job *job = ecr_job_new();
    job->command = "uname -a";
    job->description = "get host operating system";
    job->has_source_code = false;
    job->id = "123";
    job->source_code = "";

    status_info *status = storeJob("3", job);
    ecr_job_destroy(&job);

    if (status->code == REDIS_STATUS_SUCCESS) {
      ecr_job *job = retrieveJob("3");
      printf("got job: %s\n", job->description);
      ecr_job_destroy(&job);
    }
    status_info_destroy(&status);
  } else {
    printf("REDIS ERROR: %s\n", redis_info->message);
  }
  status_info_destroy(&redis_info);
  deinitRedis();
  return 0;
}
