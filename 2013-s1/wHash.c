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
    /* Get the data and output file name */
    char *data_file = argv[1];
    char *out_file = argv[2];
    /* Get the page size */
    int p_size = atoi(argv[3]);
    FILE *fp,*op;
    int records_read = 0;
    int pages,h,id,records_per_p,l,i;
    int *p_count;
    char *t;
    unsigned char *r;
    char line[MAX_LINE + 1];

    /* Open the data and out file */
    openFile(data_file,&fp,"r");
    openFile(out_file,&op,"wb");

    while(fgets(line,MAX_LINE + 1, fp) != NULL) records_read++;

    fseek(fp,0,SEEK_SET);

    /* Alloced the records per page */
    records_per_p = p_size * OCCUPANY;
    /* Total number of pages */
    pages= ceil((records_read / (float)p_size) * (1/ OCCUPANY));
    /* Count each page's records */
    p_count = calloc(pages,sizeof(int));
    checkMemory(p_count);

    /* Set up the room for the header */
    h = sizeof(int) * (pages + 1);
    fseek(op,h,SEEK_SET);
    while(fgets(line,MAX_LINE + 1, fp) != NULL){
        /* Remove newline character */
        line[strlen(line)-1] = '\0';
        r = recordByLine(line);
        id = hexTOInt(r+ID_LEFT,ID_LENGTH);
        /* Get the id from hash */
        id %= pages;
        if (p_count[id] != records_per_p) {
            /* Seek the page location */
            l = h + (RECORD_LENGTH * p_size * id) + (p_count[id] * 
                    RECORD_LENGTH);
            fseek(op,l,SEEK_SET);
            /* Write the record out */
            fwrite(r,1,RECORD_LENGTH,op);
            p_count[id]++;
        }else{
            fprintf(stderr,"Cannot write record for %d, bucket occupany\n",id);
        }
        free(r);
    }
    /* Write the header out */
    fseek(op,0,SEEK_SET);
    t = convertToHex(pages);
    fwrite(t,1,sizeof(int),op);

    /* Clean up */
    free(t);
    for (i = 0; i < pages; i++) {
        t = convertToHex(p_count[i]);
        fwrite(t,1,sizeof(int),op);
        free(t);
    }
    free(p_count);
    /* Close the file */
    fclose(fp);
    fclose(op);
    return 0;
}
