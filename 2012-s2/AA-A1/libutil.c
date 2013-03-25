
#include "libutil.h"

/*
 * safe_malloc ()
 * 
 * Call calloc () and abort if the specified amount of memory cannot be
 * allocated.
 */
    void*
safe_malloc(size_t size)
{
    void *n = calloc(1,size);
    if (n == NULL){
        fprintf(stderr, "Do not have enough memory--safe_malloc\n");
        exit(EXIT_FAILURE);
    }
    return n;

}

/*
 * safe_realloc ()
 * 
 * Call realloc () and abort if the specified amount of memory cannot be
 * allocated.
 */
    void*
safe_realloc(void *old_mem, size_t new_size)
{
    old_mem = realloc(old_mem,new_size);
    if (old_mem == NULL)
    {
        fprintf(stderr, "Do not have enough memory--safe_realloc\n");
        exit(EXIT_FAILURE);
    }
    return old_mem;
}

/*
 * safe_strdup ()
 * 
 * Safe version of strdup avoid buffer overflow, etc.
 * 
 */
    char*
safe_strdup(const char *str)
{
    char *copy = NULL;
    if (str == NULL)
    {
        fprintf(stderr, "Can not copy string in strdup\n");
        exit(EXIT_FAILURE);
    }
    copy = safe_malloc((strlen(str) + 1) * sizeof(char));
    (void) strcpy(copy,str);
    return copy;
}

/* 
 * print_info()
 *
 * return the command-line Options
 *
 */
    void
print_info(char *pname)
{
    fprintf(stdout,"%s -n nethod -i input_file -s search_file r runs\n",pname);
    fprintf(stdout,"where\n");
    fprintf(stdout,"-m [method]        = linear,binary,interpolation,exponential,block\n");
    fprintf(stdout,"-i [input_file]    = name of integer sequence file containing the (pre-ordered) set members\n");
    fprintf(stdout,"-s [search_file]   = name of interger sequence file containing the random query keys \n");
    fprintf(stdout,"-r [runs]          = then number fo times each search should be repeated\n");
}

/*
 * get_search()
 *
 * chose the search function that user input
 *
 */
    int
get_search(char *name)
{
    /* chose the search function */
    int i;
    if(strcmp(name, "linear")==0)
        i = 1;
    else if(strcmp(name, "binary")==0)
        i = 2;
    else if(strcmp(name, "interpolation")==0)
        i = 3;
    else if(strcmp(name, "exponential")==0)
        i = 4;
    else if(strcmp(name, "block")==0)
        i = 5;
    else
        i = -1;
    return i;
}

/*
 * get_time()
 *
 * get the local time
 */
uint64_t
get_time(){
    struct timeval t;
    gettimeofday(&t,NULL);
    return(t.tv_sec*SEC + t.tv_usec);
}

/*
 * get_avg()
 *
 * get the average of the time array
 */
double
get_avg(uint64_t *times,int runs){
    uint64_t sum,avg;
    int i;
    sum = 0;
    for(i =0;i<runs;i++){
        sum += times[i];
    }
    avg = sum/runs;
    return (double)avg/SEC;
} 

/*
 * get_stdev()
 *
 * get the standard deviation of the times
 */
    double
get_stdev(uint64_t *times,int runs)
{
    uint64_t sum,stdev,sum_square,avg;
    int i;
    sum = 0;
    sum_square = 0;
    for(i=0;i<runs;i++){
        sum += times[i];
        sum_square += (times[i]*times[i]);
    }
    avg = sum/runs;
    stdev = sqrt((sum_square/runs) - (avg*avg));
    return (((double)(stdev)) / SEC);
}

