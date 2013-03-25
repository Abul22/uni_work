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

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef SEC
#define SEC 1e6
#endif
#ifndef PATH_LENGTH 
#define PATH_LENGTH 50
#endif

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif
#ifndef ABS
#define ABS(x) (((x) < 0) ? -(x) : + (x) )
#endif

void           *safe_malloc(size_t size);
void           *safe_realloc(void *old_mem, size_t new_size);
char           *safe_strdup(const char *str);
/** The function build by myself*/
void           print_info(char *pname);
uint64_t       *copy_set(uint64_t *set,uint64_t len,int64_t start);
uint64_t       get_length(uint64_t array);
uint64_t       get_time();
double         get_avg(uint64_t *time,int runs);
double         get_stdev(uint64_t *time,int runs);
int            get_search(char*name);
#ifdef __cplusplus
}
#endif				/** cplusplus **/

#endif				/** _LIBUTIL_H_ **/
