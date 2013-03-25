#include "libutil.h"
#include "libset.h"

int
main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    uint64_t key_found;
    uint64_t b;
    int i,sucess;
    char *input_file = "test3.dat";
    char *search_file = "test3.qry";
    /* test set_create */
    set_t *set = set_create(DEFAULT_SET_SIZE);
    set_t *qset = set_create(DEFAULT_SET_SIZE);

    /* test set funtionality */
    read_data(set,input_file);
    read_data(qset,search_file);

    /* test set_freeze */ 
    sucess = set_freeze(set);
    if(sucess != TRUE){
        printf("the set have not freezed !!\n");
        exit(EXIT_FAILURE);
    }
    sucess = set_freeze(qset);
    if(sucess != TRUE){
        printf("the set have not freezed !!\n");
        exit(EXIT_FAILURE);
    }

    /*
    for(i = 0;i <(int)qset->items;i++){
        sucess = linear_search(set->data,set->items,qset->data[i]);
        if(sucess != -1)
            key_found++;
    }
    printf("Start linear search ...\n");
    printf("key match: %llu\n",key_found);
    */

    /* test binary search */
    printf("Start binary search ...\n");
    key_found = 0;
    for(i = 0;i <(int)qset->items;i++){
        sucess = binary_search(set->data,set->items,qset->data[i]);
        if(sucess != -1)
            key_found++;
    }
    printf("key match: %llu\n",key_found);

    /* test interpolation search */
    printf("Start interpolation search ...\n");
    key_found = 0;
    for(i = 0;i <(int)qset->items;i++){
        sucess = interpolation_search(set->data,set->items,qset->data[i]);
        if(sucess != -1)
            key_found++;
    }
    printf("key match: %llu\n",key_found);

    /* test exponential search */
    printf("Start exponential search ...\n");
    key_found = 0;
    for(i = 0;i <(int)qset->items;i++){
        sucess = exponential_search(set->data,set->items,qset->data[i]);
        if(sucess != -1)
            key_found++;
    }
    printf("key match: %llu\n",key_found);

    /* test block search */
    printf("Start block search ...\n");
    key_found = 0;
    /* b = (uint64_t)floor(log(qset->items)); */
    b = (uint64_t)sqrt(qset->items);
    for(i = 0;i <(int)qset->items;i++){
        sucess = block_search(set->data,set->items,qset->data[i],b);
        if(sucess != -1)
            key_found++;
    }
    printf("key match: %llu\n",key_found);

	return EXIT_SUCCESS;
}
