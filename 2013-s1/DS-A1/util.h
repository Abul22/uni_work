/***************************************************************************
 *
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/*#include <sys/time.h>*/
#define MAX_LINE            80
#define N_FIELDS            5 /*number of the fields in the struct */

#define RECORD_LENGTH       53
/* Name */
#define NAME_LENGTH         17
/* Species */
#define SPECIES_LENGTH      1
#define SPECIES_LEFT        17
/* Class */
#define CLASS_LENGTH        1
#define CLASS_LEFT          18
/* ID */
#define ID_LENGTH           4
#define ID_LEFT             19 
/* Guild */
#define GUILD_LENGTH        30
#define GUILD_LEFT          23

#define OCCUPANY            (0.8)

typedef struct{
    char name[NAME_LENGTH+1];
    unsigned char species;
    unsigned char class;
    unsigned int id;
    char guild[GUILD_LENGTH+1];
}record_t;

/* Easy access the field to these number */
enum record{NAME,SPECIES,CLASS,ID,GUILD};

/* Vaild check function */
void checkMemory(void *p);
void argCheck(int argc, char **argv);
void openFile(char *fname,FILE**fp,char*mode);

/*Read function */
unsigned char* recordByLine(char* line);
void readRecords(FILE *fp,int records,record_t **r);

/*Hex with Int convert */
char* convertToHex(int n);
int hexTOInt(unsigned char *c, int len);
