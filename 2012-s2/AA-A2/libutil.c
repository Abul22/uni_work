/***************************************************************************
 *
 *   File        : libutil.c
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
#include "libutil.h"

/*
 * safe_malloc ()
 * 
 * Call calloc () and abort if the specified amount of memory cannot be
 * allocated.
 */

    void           *
safe_malloc(size_t size)
{
    void *mem_block = NULL;

    if ((mem_block = calloc(1, size)) == NULL) {
        fprintf(stderr, "ERROR: safe_malloc(%zu) cannot allocate memory.", size);
        exit(EXIT_FAILURE);
    }
    return (mem_block);
}

/*
 * safe_realloc ()
 * 
 * Call realloc () and abort if the specified amount of memory cannot be
 * allocated.
 */

    void *
safe_realloc(void *old_mem, size_t new_size)
{
    if ((old_mem = realloc(old_mem, new_size)) == NULL) {
        fprintf(stderr, "ERROR: safe_realloc() cannot allocate"
                "%zu blocks of memory.\n", new_size);
        exit(EXIT_FAILURE);
    }
    return (old_mem);
}

/*
 * safe_strdup ()
 * 
 * Safe version of strdup avoid buffer overflow, etc.
 * 
 */

    char *
safe_strdup(const char *str)
{
    char *copy = NULL;

    if (str == NULL) {
        fprintf(stderr, "ERROR safe_strdup(): str == NULL");
        exit(EXIT_FAILURE);
    }
    copy = safe_malloc((strlen(str) + 1) * sizeof(char));

    (void) strcpy(copy, str);

    return (copy);
}

    char *
safe_strcat(char *str1, const char *str2)
{
    char *rv = NULL;
    size_t len = 0;

    if (str1 == NULL || str2 == NULL) {
        fprintf(stderr, "ERROR safe_strcat_new(): str == NULL");
        exit(EXIT_FAILURE);
    }
    len = strlen(str1) + strlen(str2) + 1;
    rv = safe_malloc(len * sizeof(char));
    (void) strcpy(rv, str1);
    rv = strcat(rv, str2);

    return (rv);
}

/* calculate the average */
    float
calculate_avg(hrtime_t * runtimes, int n)
{
    int i;
    hrtime_t total = 0;

    for (i = 0; i < n; i++) {
        total += runtimes[i];
    }

    /* convert to right format */
    return (((float) (total / n)) / 1000000000);
}

    float
calculate_stddev(hrtime_t * runtimes, int n)
{
    int i;
    hrtime_t total = 0;
    hrtime_t total_square = 0;
    hrtime_t stddev;
    hrtime_t avg;

    for (i = 0; i < n; i++) {
        total += runtimes[i];
        total_square += (runtimes[i] * runtimes[i]);
    }

    avg = total / n;
    stddev = sqrt((total_square / n) - (avg * avg));

    return (((float) (stddev)) / 1000000000);
}
