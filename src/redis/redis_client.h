#include "redis_client_private.h"

redis_client* redis_client_new();
void redis_client_destroy(redis_client **client);