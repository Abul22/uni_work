/***************************************************************************
 *
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
#include "util.h"
int main(int argc, char **argv)
{

    /* Check the argument */
    argCheck(argc,argv);
    /* Get the search and heap file name */
    char* search_file = argv[1];
    char* heap_file = argv[2];
    /* Get the page size */
    int p_size = atoi(argv[3]);
    FILE *sp,*hp;
    int i;
    char line[MAX_LINE+1];
    unsigned int f_id,found;
    record_t **r;
    hrtime_t start, end;

    /* Open the data and out file */
    openFile(search_file,&sp,"r");
    openFile(heap_file,&hp,"rb");

    /* allocte the record sets base on page size */
    r = malloc(p_size * sizeof(record_t*));
    checkMemory(r);
    for (i = 0; i < p_size; i++) {
        r[i] = calloc(1,sizeof(record_t));
        checkMemory(r[i]);
    }
    start = gethrtime();
    while(fgets(line,MAX_LINE+1,sp) != NULL){
        found = 0;
        f_id = atoi(line);
        while(!feof(hp) && !found){
            /* Read record and search for match */
            readRecords(hp, p_size, r);
            for (i = 0; i < p_size; i++) {
                /* Base on ID to make sure find result */
                if(r[i] && (f_id == r[i]->id)){
                    printf("%s,%d,%d,%d,%s\n\n",r[i]->name, r[i]->species, 
                            r[i]->class, r[i]->id,r[i]->guild);
                    found = 1;
                    break;
                }
            }
        }
        /* Have not found result */
        if(!found)
            fprintf(stderr,"Can not find match for %d\n",f_id);
        /* Back the beaginning of the file */
        fseek(hp,0,SEEK_SET);
    }
    end = gethrtime();
    printf("Time: %.2f ms\n",(end - start)/1000000.0);
    /* Clean up */
    for (i = 0; i < p_size; i++) {
        if(r[i])
            free(r[i]);
    }
    free(r);
    /* Close the file */
    fclose(hp);
    fclose(sp);
    return 0;
}
