/*
 * @CreateTime: Apr 14, 2019 9:46 PM
 * @Author: Harris Brakmic
 * @Contact: brakmic@gmail.com
 * @Last Modified By: brakmic
 * @Last Modified Time: Apr 14, 2019 9:47 PM
 * @Description: Execute Collect Report -  A tool for remote code execution
 */


#include "src/ecr/base/base.h"
/**
 * @brief  Test program to show the capabilities of ECR
 * @note   
 * @param  argc: argument count
 * @param  **argv: arguments in vector of strings
 * @retval status
 */
int main(int argc, char **argv) {

  redis_client *client = redis_client_new();
  status_info *redis_info = client->connect("localhost", 6379, false);

  if (redis_info->code == REDIS_STATUS_SUCCESS) {
    ecr_job_data *job_data = client->create_job_data("print()", true, LANG_UNKNOWN);
    ecr_job *job = client->create_job("123", "get host operating system", job_data);
    assert(job);
    status_info *status = client->store_job(job);
    ecr_job_destroy(&job);

    if (status->code == REDIS_STATUS_SUCCESS) {
      ecr_job *job = client->retrieve_job("123");
      assert(job);
      printf("Retrieved Job: %s\n", job->description);
      client->remove_job(strndup(job->id, strlen(job->id)));
      ecr_job_destroy(&job);
    }
    status_info_destroy(&status);
  } else {
    printf("REDIS ERROR: %s\n", redis_info->message);
  }
  status_info_destroy(&redis_info);
  client->disconnect();
  return 0;
}
