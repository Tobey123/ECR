#include <criterion/criterion.h>

#undef true
#undef false
#include <stdbool.h>

#include "../src/cJSON/cJSON.h"
#include "../src/ecr/status/status.h"
#include "../src/ecr/job/job.h"
#include "../src/redis/redis_client.h"

#include "../src/cJSON/cJSON.c"
#include "../src/ecr/status/status.c"
#include "../src/ecr/job/job.c"
#include "../src/redis/redis_client.c"

/*
* Testing status
*/

Test(creates_status_object, passing) {
    status_info *info = status_info_new();
    cr_assert(info);
    status_info_destroy(&info);
}

Test(destroys_status_object, passing) {
    status_info *info = status_info_new();
    status_info_destroy(&info);
    cr_assert_not(info);
}

Test(status_info_properties_are_accessible, passing) {
    status_info *info = status_info_new();
    info->code = REDIS_STATUS_SUCCESS;
    info->message = "message";
    cr_assert_eq(info->code, REDIS_STATUS_SUCCESS);
    cr_assert_str_eq(info->message, "message");
    status_info_destroy(&info);
}

/*
* Testing job
*/

Test(creates_job_object, passing) {
    ecr_job *job = ecr_job_new();
    cr_assert(job);
    ecr_job_destroy(&job);
}

Test(destroys_job_object, passing) {
    ecr_job *job = ecr_job_new();
    ecr_job_destroy(&job);
    cr_assert_not(job);
}

Test(job_properties_are_accessible, passing) {
    ecr_job *job = ecr_job_new();
    job->command = "command";
    job->description = "description";
    job->has_source_code = true;
    job->id = "1";
    job->source_code = "print()";
    cr_assert_str_eq(job->command, "command");
    cr_assert_str_eq(job->description, "description");
    cr_assert_eq(job->has_source_code, true);
    cr_assert_str_eq(job->id, "1");
    cr_assert_str_eq(job->source_code, "print()");
    ecr_job_destroy(&job);
}

/*
* Testing redis
*/

Test(create_redis_client, passing) {
    redis_client *client = redis_client_new();
    cr_assert(client);
    redis_client_destroy(&client);
}

Test(destroy_redis_client, passing) {
    redis_client *client = redis_client_new();
    cr_assert(client);
    redis_client_destroy(&client);
    cr_assert_not(client);
}

Test(initialize_redis_connection, passing) {
    redis_client *client = redis_client_new();
    status_info *info = client->connect("localhost", 6379, false);
    cr_assert_eq(info->code, REDIS_STATUS_SUCCESS);
    client->disconnect();
    status_info_destroy(&info);
    redis_client_destroy(&client);
}

Test(deinitialize_redis_connection, passing) {
    redis_client *client = redis_client_new();
    status_info *info = client->connect("localhost", 6379, false);
    status_info *info2 = client->disconnect();
    cr_assert_eq(info2->code, REDIS_STATUS_SUCCESS);
    status_info_destroy(&info);
    status_info_destroy(&info2);
}

redis_client *client = NULL;
void setup_redis(void) {
    client = redis_client_new();
    status_info *info = client->connect("localhost", 6379, false);
}

void teardown_redis(void) {
    client->disconnect();
    redis_client_destroy(&client);
}

Test(create_job, passing, .init = setup_redis, .fini = teardown_redis) {
    ecr_job *job = client->create_job("123", "description", "", "print()", false);
    cr_assert(job);
    ecr_job_destroy(&job);
}

Test(remove_job, passing, .init = setup_redis, .fini = teardown_redis) {
    ecr_job *job = client->create_job("123", "description", "", "print()", false);
    status_info *job_status = client->store_job(job);
    cr_assert_eq(job_status->code, REDIS_STATUS_SUCCESS);
    status_info *removal_status = client->remove_job(strdup(job->id));
    cr_assert_eq(removal_status->code, REDIS_STATUS_SUCCESS);
    status_info_destroy(&job_status);
    ecr_job_destroy(&job);
}

Test(store_job, passing, .init = setup_redis, .fini = teardown_redis) {
    ecr_job *job = client->create_job("123", "description", "", "print()", false);
    status_info *job_status = client->store_job(job);
    cr_assert_eq(job_status->code, REDIS_STATUS_SUCCESS);
    client->remove_job(strdup(job->id));
    status_info_destroy(&job_status);
    ecr_job_destroy(&job);
}

