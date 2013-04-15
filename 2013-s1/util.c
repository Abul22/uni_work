/***************************************************************************
 *
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
#include "util.h"
/*
 * argCheck()
 * Check the argc and argv
 */
void argCheck(int argc, char **argv){
    /* Check the argument number */
    if (argc < 4) {
        fprintf(stderr,"wHeap: ./wHeap data_2013 heap pagesize\n");
        fprintf(stderr,"sHeap: ./sHeap search_2013 heap pagesize\n\n");
        fprintf(stderr,"wHash: ./wHash data_2013 hashed pagesize\n");
        fprintf(stderr,"sHash: ./sHash search_2013 hashed pagesize\n\n");
        exit(1);
    }

    /* Check the page size, make sure it is greater than 0 */
    if (atoi(argv[3]) <= 0) {
        fprintf(stderr,"Please make sure the pages size is greater than 0\n");
        exit(1);
    }
}

/*
 * checkMemory()
 * Make sure the memory space
 */
void checkMemory(void *p){
    if (p==NULL) {
        fprintf(stderr,"Out of memory!\n");
        exit(1);
    }
}

/* 
 * openFile()
 * open the file safely
 */
void openFile(char *fname,FILE**fp,char*mode){
    *fp = fopen(fname,mode);
    if (*fp == NULL) {
        fprintf(stderr,"Cannot open the %s",fname);
        exit(1);
    }
}

/* 
 * recordByLine()
 * read data line by line retrun the charater arrray
 */
unsigned char* recordByLine(char* line){
    unsigned char species,class;
    unsigned int id;
    char* fields[N_FIELDS];
    int i = 0;
    int f = 0;
    int s = 0;
    int j;

    fields[i++] = strtok(line,",");
    while(fields[i-1] != NULL && i < N_FIELDS)
        fields[i++] = strtok(NULL,",");

    unsigned char *c = malloc(RECORD_LENGTH);
    checkMemory(c);

    /* Name  */
    j = strlen(fields[NAME]);
    for (i = 0; i < NAME_LENGTH; i++) {
        c[i] = (i < j) ? fields[NAME][i] : '\0';
    }

    /* Species */
    species = atoi(fields[SPECIES]);
    for (f = 0, s = (SPECIES_LENGTH-1) * 8; f < SPECIES_LENGTH;i++,s-=8, f++) {
        c[i] = (species >> s) & 0xFF;
    }

    /* Class */
    class  = atoi(fields[CLASS]);
    for (f = 0, s = (CLASS_LENGTH-1) * 8; f < CLASS_LENGTH;i++,s-=8, f++) {
        c[i] = (class >> s) & 0xFF;
    }

    /* ID */
    id  = atoi(fields[ID]);
    for (f = 0, s = (ID_LENGTH-1) * 8; f < ID_LENGTH;i++,s-=8, f++) {
        c[i] = (id >> s) & 0xFF;
    }

    /* Guild */
    j = fields[GUILD] ? strlen(fields[GUILD]) : -1;
    for (f = 0; f < GUILD_LENGTH; f++) {
        c[i+f] = (f < j) ? fields[GUILD][f] : '\0';
    }

    return c;
}

/*
 * readRecords()
 * converts the records in binary to records
 */
void readRecords(FILE *fp,int records,record_t **r){
    /* Read data */
    unsigned char data[RECORD_LENGTH * records];
    int read = fread(data,RECORD_LENGTH,records,fp);

    /* Convert to records */
    int nr, i;
    for (i = 0; i < read; i++) {
        nr = i * RECORD_LENGTH;
        /* Name  */
        memcpy(r[i]->name,data + nr, NAME_LENGTH);
        r[i]->name[NAME_LENGTH] = '\0';
        /* Species */
        r[i]->species = hexTOInt(data + nr + SPECIES_LEFT,SPECIES_LENGTH );
        /* Class */
        r[i]->class = hexTOInt(data + nr + CLASS_LEFT,CLASS_LENGTH);
        /* ID */
        r[i]->id = hexTOInt(data + nr + ID_LEFT,ID_LENGTH);
        /* Guild */
        memcpy(r[i]->guild,data + nr + GUILD_LEFT, GUILD_LENGTH);
        r[i]->guild[GUILD_LENGTH] = '\0';
    }
}

/*
 * convertToHex()
 * Convert interger to hex characters
 */
char* convertToHex(int n){
    int i,s;
    int len = sizeof(int);
    char *c = malloc(len);
    for (i = 0, s = 8 *(len-1); i < len;i++,s -= 8) {
        c[i] = (n >> s) & 0xFF;
    }
    return c;
}

/* 
 * hexTOInt()
 * convert hex characters to interger
 */
int hexTOInt(unsigned char *c, int len){
    int n = 0;
    int i;
    for (i = 0; i < len; i++) {
        n <<= 8;
        n |= c[i];
    }
    return n;
}
