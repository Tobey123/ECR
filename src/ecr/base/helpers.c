#ifndef __ECR_HELPERS__
#define __ECR_HELPERS__

#include <stdlib.h>
#include <string.h>

static char *_strdup(const char *str);

/**
 * @brief  Duplicate string
 * @note   
 * @param  *str: string to be duplicated
 * @retval duplicated string
 */
char *_strdup(const char *str)
{
  size_t len;
  char *newstr;

  if(!str)
    return (char *)NULL;

  len = strlen(str);

  if(len >= ((size_t)-1) / sizeof(char))
    return (char *)NULL;

  newstr = (char*)malloc((len + 1)*sizeof(char));
  if(!newstr)
    return (char *)NULL;

  memcpy(newstr, str, (len + 1)*sizeof(char));

  return newstr;

}

#endif
