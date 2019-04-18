#ifndef __ECR_DEBUG__
#define __ECR_DEBUG__

#include <stdlib.h>
#include <stdio.h>

extern long alloc_count;

static void* ecr_debug_malloc(size_t size, char *file, unsigned int line);
static void ecr_debug_free(void *ptr, char *file, unsigned int line);
static void ecr_debug_update_alloc_counter(long alloc);
static long ecr_debug_get_allocation_counter();

void* ecr_debug_malloc(size_t size, char *file, unsigned int line) {
  ecr_debug_update_alloc_counter(1);
  printf("DEBUG | ALLOC | FILE: %5s | LINE: %5d\n", file, line);
  return malloc(size);
}

void ecr_debug_free(void *ptr, char *file, unsigned int line) {
  ecr_debug_update_alloc_counter(-1);
  printf("DEBUG | FREE | FILE: %5s | LINE: %5d\n", file, line);
  free(ptr);
}

void ecr_debug_update_alloc_counter(long alloc) {
  alloc_count += alloc;
}

long ecr_debug_get_allocation_counter() {
  return alloc_count;
}

#ifdef ECR_DEBUG

  #define malloc(n)  ecr_debug_malloc(n, __FILE__, __LINE__)
  #define free(n)    ecr_debug_free(n, __FILE__, __LINE__)

#endif

#endif //__ECR_DEBUG__
