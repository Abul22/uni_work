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
    char * data_file = argv[1];
    char * out_file = argv[2];
    /* Get the page size */
    int p_size = atoi(argv[3]);
    FILE *fp,*op;
    unsigned char records[p_size * RECORD_LENGTH]; 
    unsigned char *r;
    int records_read = 0;
    char line[MAX_LINE + 1];

    /* Open the data and out file */
    openFile(data_file,&fp,"r");
    openFile(out_file,&op,"wb");

    while(fgets(line,MAX_LINE + 1, fp) != NULL){
        /* Remove newline character */
        line[strlen(line)-1] = '\0';
        r = recordByLine(line);
        memcpy(records+(records_read++ * RECORD_LENGTH),r,RECORD_LENGTH);
        free(r);
        if (records_read == p_size) {
            /* Write the record out */
            fwrite(records,1,RECORD_LENGTH*p_size,op);
            records_read = 0;
        }
    }
    /* Write remaining record out */
    if (records_read != 0) {
        fwrite(records,1,RECORD_LENGTH*p_size,op);
    }
    /* Close the file */
    fclose(fp);
    fclose(op);
    return 0;
}
