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
    /* Get the search and hash file name */
    char* search_file = argv[1];
    char* hash_file = argv[2];
    /* Get the page size */
    int p_size = atoi(argv[3]);
    int * records_count;
    int pages,i,h;
    unsigned char temp[sizeof(int)];
    unsigned int f_id,found, h_id;
    FILE *sp,*hp;
    char line[MAX_LINE+1];
    record_t **r;
    hrtime_t start, end;

    /* Open the search and hash file */
    openFile(search_file,&sp,"r");
    openFile(hash_file,&hp,"rb");

    /* Read the header */
    fread(temp,sizeof(int),1,hp);
    /* Number of pages */
    pages = hexTOInt(temp,sizeof(int));
    /* Number of records in a page */
    records_count = malloc(pages * sizeof(int));
    checkMemory(records_count);
    for (i = 0; i < pages; i++) {
        fread(temp,sizeof(int),1,hp);
        records_count[i] = hexTOInt(temp,sizeof(int));
    }
    h = sizeof(int) * (pages+1);
    r = malloc(p_size * sizeof(record_t));
    checkMemory(r);
    for (i = 0; i < p_size; i++) {
        r[i] = calloc(1,sizeof(record_t));
        checkMemory(r[i]);
    }
    satart = gethrtime();
    while(fgets(line, MAX_LINE + 1, sp) != NULL){
        f_id = atoi(line);
        h_id = f_id % pages;
        found = 0;
        /* Seek the page location */
        fseek(hp,h+(RECORD_LENGTH * h_id * p_size),SEEK_SET);
        /* Read record form page */
        readRecords(hp,records_count[h_id],r);
        for (i = 0; i < records_count[h_id]; i++) {
                /* Base on ID to make sure find result */
            if (r[i] && f_id == r[i]->id) {
                printf("%s,%d,%d,%d,%s\n\n",r[i]->name, r[i]->species, 
                        r[i]->class, r[i]->id,r[i]->guild);
                found = 1;
                break;
            }
        }
        /* Have not found result */
        if(!found)
            fprintf(stderr,"Can not find match for %d\n",f_id);
    }
    end = gethrtime();
    printf("Time: %.2f ms\n",(end - start)/1000000.0);
    /* Clean up */
    for (i = 0; i < p_size; i++) {
        if (r[i]) 
            free(r[i]);
    }
    free(r);
    free(records_count);
    /* Close the file */
    fclose(hp);
    fclose(sp);
    return 0;
}
