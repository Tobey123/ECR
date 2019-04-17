#include "redis_client_private.c"
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
  client->create_job_data = prv_create_job_data;
  client->remove_job = prv_remove_job;
  client->retrieve_job = prv_retrieve_job;
  client->store_job = prv_store_job;
  return client;
}
