#include "libset.h"
#include <assert.h>

/** Basic Set Operations **/

/* use unsigned int. will overflow for linear search for big files otherwise */
static uint64_t cmp_cnt;

/* reset the comparison counter */
    void
reset_cmp_cnt()
{
    cmp_cnt = 0;
}

/* return the value of the comparison counter */
    uint64_t
get_cmp_cnt()
{
    return cmp_cnt;
}

/* increase the comparison counter */
    static void
inc_cmp_cnt()
{
    cmp_cnt++;
}

/*
 * insert a new item into the set and adjust the size of the underlying
 * dynamic memory block if needed
 */
    int
set_insert(set_t * set, uint64_t item)
{
    assert(set != NULL);
    assert(set->data !=NULL); 

    /* check if the set have locked or not */
    if(set->lock != TRUE){
        /* if the size is not enough, double it */
        if (set->size == set->items)
        {	
            set->size = set->size * 2;
            /* realloc the memory if the size have changed */
            set->data = safe_realloc(set->data,sizeof(uint64_t) * set->size);
        }
        /* add the item into array */
        set->data[set->items] = item;
        /* the max number will be the last one */
        set->n_max = set->data[set->items];
        /* add the number of the itmes */
        set->items = set->items + 1;
        return TRUE;
    }
    else{
        fprintf(stderr,"Can not insert the data because it already locked!!\n");
        return FALSE;
    }
}

/*
 * create and initialize a set_t data structure of size 'size'
 */
    set_t*
set_create(uint64_t size)
{
    set_t *set = (set_t *) safe_malloc(sizeof(set_t));
    /*initialize all the element in set_t*/
    set->size = size;
    set->items = 0;
    set->n_max = 0;
    set->lock = FALSE;
    /* malloc the data array by defalut size */
    set->data = safe_malloc(sizeof(uint64_t)*size);
    return set;
}

/* free all memory used by the set */
    void
set_destroy(set_t * set)
{
    assert(set != NULL);
    if(set->data)
        /* free the data array */
        free(set->data);
    /* free the set */
    free(set);
}

/*
 * freeze the set - reallocate the dynamic memory block in the set to a
 * contiguous memory block of the exact size of the array and 'lock' the data
 * structure
 */
    int
set_freeze(set_t * set)
{
    assert(set !=NULL);
    /* relalloc the set's memory and locked it  */
    set = safe_realloc(set,sizeof(set_t));
    set->lock = TRUE;
    return TRUE;
}

/*
 * perform Algorithm 'LinearSearch' on the set and if 'key' is found in the
 * set return it's index otherwise return -1
 */
    int64_t
linear_search(uint64_t *set, uint64_t len, uint64_t key)
{
    int64_t i;
    for(i = 0;i<(int64_t)len;i++){
        if(set[i] == key){
            return i;
        }
        else if(set[i] > key){
            return -1;
        }
        inc_cmp_cnt();
    }
    return -1; 
}

/*
 * perform Algorithm 'BinarySearch' on the set and if 'key' is found in the
 * set return it's index otherwise return -1
 */
    int64_t
binary_search(uint64_t *set, uint64_t len, uint64_t key)
{
    int64_t start, middle, end;
    start = 0;
    end =  len - 1;
    while(start <= end){
        middle = (int64_t)floor(start+(end-start) / 2);
        if(set[middle] == key){
            return middle;
        }else if(key < set[middle]){
            end = middle - 1;
        }else{
            start = middle + 1;
        }
        inc_cmp_cnt();
    }
    return -1;
}

/*
 * perform Algortihm 'InterPolationSearch' on the set and if 'key' is found
 * in the set return it's index otherwise return -1
 */
    int64_t
interpolation_search(uint64_t *set, uint64_t len, uint64_t key)
{
    int64_t low,middle,high;
    low = 0;
    high = len - 1;
    while((set[low] < key) && (set[high] >= key)){
        middle = low + ((high - low) * (key - set[low])) / (set[high] - set[low]);
        if(set[middle] < key)
            low = middle + 1;
        else if(set[middle] > key)
            high = middle - 1;
        else
            return middle;
        inc_cmp_cnt();
    }
    if (set[low] == key){
        return low;
        inc_cmp_cnt();
    }
    else
        return -1;
}

/*
 * perform Algorithm 'ExponentialSearch' on the set and if 'key' is found in
 * the set return it's index otherwise return -1
 */
    int64_t
exponential_search(uint64_t *set,uint64_t len, uint64_t key)
{
    int64_t pos = 2;
    int64_t prev,result;
    uint64_t newLen;
    while((pos < (int64_t)len) && (set[pos] < key)){
        prev = pos;
        pos = pos * 2;
        inc_cmp_cnt();
    }
    if(pos > (int64_t)len - 1){
        pos = len - 1;
    }
    newLen = (pos + 1) - prev;
    result = binary_search(&set[prev],newLen,key);
    if(result == -1){
        return -1;
    }
    else{
        return result + prev;
    }
}

/*
 * perform Algorithm 'BlockSearch' on the set and if 'key' is found in the
 * set return it's index otherwise return -1
 * 
 * parameter 'b' referes to a fixed block size of probe positions
 */
    int64_t
block_search(uint64_t *set, uint64_t len, uint64_t key, uint64_t b)
{
    int64_t pos = (uint64_t)b;
    int64_t prev,result;
    uint64_t newLen;
    while((pos < (int64_t)len) && (set[pos] < key)){
        prev = pos;
        pos = pos + (int64_t)b;
        inc_cmp_cnt();
    }
    if(pos > (int64_t)len - 1){
        pos = len - 1;
    }
    newLen = (pos + 1) - prev;
    result = binary_search(&set[prev],(uint64_t)newLen,key);
    if (result == -1)
        return -1;
    else
        return result + prev;
}

/*
 * read_data()
 *
 * read data from a file to a set
 *
 */
    void
read_data(set_t *pset,char *pname)
{
    FILE *fp;
    uint64_t item;
    int sucess; /* use to check if sucess insert the item into set or not */
    /* the defalut path for the test data */
    char path[PATH_LENGTH] = "/public/courses/AlgorithmsAndAnalysis/proj1/";
    /*  put the file name with defalut path together */
    pname = strcat(path,pname);
    /* safe copy the whole path */
    pname = safe_strdup(pname);
    fp = fopen(pname,"r");
    /* check if file exit or not */
    if(!fp){
        fprintf(stderr,"Can not open %s!!!\n",pname);
        exit(EXIT_FAILURE);
    }
    /* get the numbers to the end of file */
    while(fscanf(fp,"%llu",&item) == 1 ){
        sucess = set_insert(pset,item);
        /* exit if have not sucess insert the number */
        if(sucess != TRUE){
            fprintf(stderr,"Can not insert the data in the set!!\n");
            exit(EXIT_FAILURE);
        }
    }
    /* close the file */
    fclose(fp);
}
