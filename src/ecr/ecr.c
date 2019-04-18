#include "ecr.h"
#include "compat/compat.c"
#include "job/job_data.c"
#include "job/job.c"
#include "status/status.c"
#include "redis/redis_client.c"
#include "cjson/cJSON.c"

long alloc_count = 0;
