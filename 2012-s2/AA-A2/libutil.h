/***************************************************************************
 *
 *   File        : libutil.h
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
/*** Miscelaneous Utility Functions ***/
#ifndef _LIBUTIL_H_
#define _LIBUTIL_H_

#ifdef __cplusplus
extern          "C" {
#endif				/** cplusplus **/

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#define REALLOC_INCREMENT 256

#define PATH_LENGTH 60

#define USUAL_ARGC 7

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define UNUSED(x) (void)(x)
#define SWAP_BYTES(x) swap_bytes((unsigned char *)&x, sizeof(x))
#define TIMEVAL_TO_TIMET(ts,t)  (*t) = (ts)->tv_sec

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif
#ifndef ABS
#define ABS(x) (((x) < 0) ? -(x) : + (x) )
#endif

void *safe_malloc(size_t size);
void *safe_realloc(void *old_mem, size_t new_size);
char *safe_strdup(const char *str);
char *safe_strcat(char *str1, const char *str2);

float calculate_avg(hrtime_t * runtimes, int n);
float calculate_stddev(hrtime_t * runtimes, int n);

#ifdef __cplusplus
}
#endif				/** cplusplus **/

#endif				/** _LIBUTIL_H_ **/
