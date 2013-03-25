#include "libutil.h"
#include "libset.h"

    int
main(int argc, char **argv)
{
    int queries;
    /* use for the getopt() */
    int opt = -1;
    /* use for get the input mode */
    int search_opt;
    int runs,locked,i,j;
    /* b is for the block size in block search */
    uint64_t keys_found,b;
    /* the var use in get the time */
    uint64_t start,stop,total,*times;
    float avg,stdev;
    /* function pointer for search functions */
    int64_t (*mode)(uint64_t*,uint64_t,uint64_t);
    int64_t (*block)(uint64_t *,uint64_t,uint64_t,uint64_t);
    /* use the check bool return */
    int64_t scuess;
    /*input and search file*/
    char *inputFile;
    char *searchFile;
    /*the pass arguments*/
    static const char *optString = "m:i:s:r:";
    /*difine the set*/
    set_t *set, *qset;
    /*define the algorithm name */
    char *algorithmName;
    /* 1. Parse Arguments */
    if(argc <1){
        /* this is a function build by myself
         * can be found in libutil.c
         */
        print_info(argv[0]);
        exit(EXIT_FAILURE);
    }

    /* using getopt() to get each val from user input */
    while((opt = getopt(argc,argv,optString)) != -1){
        switch(opt){
            case 'm':
                /* get_search() is a function build by myself
                 * can be found in libutil.c
                 */
                search_opt = get_search(optarg);
                algorithmName = safe_strdup(optarg);
                break;
            case 'i':
                inputFile = optarg; /* will use safe_strdup in read_file() */
                break;
            case 's':
                searchFile = optarg; /* will use safe_strdup in read_file() */
                break;
            case 'r':
                runs = atoi(optarg);
                break;
            default:
                /* print the help menu if not correct */
                print_info(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    /* check each input */
    if (algorithmName == NULL || inputFile == NULL || searchFile == NULL || runs == 0){
        /* print the help menu if not correct */
        print_info(argv[0]);
        exit(EXIT_FAILURE);
    }

    /* 2. create data empty set_t */
    set = set_create(DEFAULT_SET_SIZE);

    /* 3. Use fscanf to read all data values into new set_t */

    /* this is a function build by myself 
     * can be found in libset.c
     */
    read_data(set,inputFile);

    /*
     * 4. Freeze Set - reallocate so the set uses one contiguous memory
     * block of the exact size
     */
    locked = set_freeze(set);
    /* lock check */
    if(locked != TRUE){
        fprintf(stderr,"Can not freeze the set!!!\n");
        exit(EXIT_FAILURE);
    } 

    /* 6. create empty  query set_t */
    qset = set_create(DEFAULT_SET_SIZE);

    /* 7. read the query set */

    /* this is a function build by myself 
     * can be found in libset.c
     */
    read_data(qset,searchFile); 

    locked = set_freeze(qset);
    /* lock check */
    if(locked != TRUE){
        fprintf(stderr,"Can not freeze the set!!!\n");
        exit(EXIT_FAILURE);
    } 

    /*
     * 5. For each key in the query set, run the appropriate search function
     * and time it, based on the mode. Use fscanf to read the key, and
     * only time the search cost
     */
    /* using function pointer to get the search function */
    if(search_opt == 1)
        mode = &linear_search;
    else if (search_opt == 2)
        mode = &binary_search;
    else if (search_opt == 3)
        mode = &interpolation_search;
    else if (search_opt == 4)
        mode = &exponential_search;
    else if (search_opt == 5)
        block = &block_search;
    else{
        print_info(argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("ALGORTIHM :%s\n", algorithmName);
    queries = qset->items;

    /* 8. output number of items in the data set and the number of search keys */
    fprintf(stdout,"KEYS SEARCHED: %d\n",queries);
    fprintf(stdout,"RUNS : %d\n",runs);

    /* 9. reset compare counter and allocate the runtimes array*/
    times = safe_malloc(runs*sizeof(uint64_t));
    /* run over the search depend on runs number */
    for(j = 0;j<runs;j++){
        /* reset the cmp_cnt for each runs */
        reset_cmp_cnt();
        /* reset the key found for each runs */
        keys_found = 0;
        /* reset the total time */
        total = 0;
        for(i = 0;i < (int)qset->items;i++){
            /* check if use block search or not */
            if(block == NULL){
                /* start time */
                start = get_time();
                /* get_time() is a function build by myself
                 * can be found in libutil.c
                 */
                scuess = (*mode)(set->data,set->items,qset->data[i]);
                /* end time */
                stop = get_time();
                /* incress the key found when sucess found the key */
                if(scuess != -1)
                    keys_found++;
            }
            else{
                /* for block serach set the b's value 
                 * b = (uint64_t)floor(log(qset->items));
                 */
                b = (uint64_t)sqrt(qset->items);
                /* start time */
                start = get_time();
                scuess = (*block)(set->data,set->items,qset->data[i],b);
                /* end time */
                stop = get_time();
                /* incress the key found when sucess found the key */
                if(scuess != -1)
                    keys_found++;
            }
            /* total time the search */
            total += stop - start;
        }
        /* pust the total time into times array */
        times[j] = total;
    }
    /* 10. switch the modes and do the searches */

    /* 11. output keys found and number of compares performed */
    fprintf(stdout,"KEYS MATCHED : %llu\n",keys_found);
    fprintf(stdout,"COMPARES : %llu\n",get_cmp_cnt());

    /* 12. calculate the average */
    /* calculate average and standard deviation */

    /* these functions are  build by myself
     * can be found in libutil.c
     */
    avg = get_avg(times,runs);
    stdev = get_stdev(times,runs);

    /* 13. output timings */
    fprintf(stdout,"TIME : %.5f +/- %.5f s\n",avg,stdev);

    /* 14. Make sure you free all malloc'd resources and close open files */
    /* free all sets */
    free(times);
    set_destroy(set);
    set_destroy(qset);

    return EXIT_SUCCESS;
}

