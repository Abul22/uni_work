/***************************************************************************
 *
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
#ifndef UTIL_H
#define UTIL_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>

#pragma pack(1)

#define BUFFER_SIZE       56
#define MAX_LEN           80
#define RECORD_SIZE       28
#define C_NAME_LEN        12
#define G_NAME_LEN        24
#define RECORDS_PP        2
#define RECORDS           10000
#define TIMESECORD        100000.0

#define C_STRING         "characters."
#define G_STRING         "guilds."

typedef struct {
  char name[C_NAME_LEN];
  int species;
  int class;
  int id;
  int guild_id;
} Character;

typedef struct {
  int guild_id;
  char g_name[G_NAME_LEN];
} Guild;

typedef struct{
  char *p_size;
  int buffers;
  int flag;
  char *dir;
  char *o_file;
  char *i_file;
  char *a_name;
  char *value;
  char *project;
} Opthions;

typedef union{
  Character **c;
  Guild **g;
} Page;


Page *read_page(int r_pp, FILE *fp,int is_c,int p_size);
Page** read_block(int size,FILE *fp,int is_c, int p_size);
Opthions *arg_init();
Opthions *arg_load(int argc, char** argv);
Opthions *arg_query(int argc, char** argv);
void print_usage(const char *program);
void *safe_malloc(unsigned int n);
void *safe_calloc(unsigned int n,unsigned int size);
void  check_file(char *name,FILE **fp, const char *m);
void *create_record(char *line, int is_c);
void free_page(Page *page, int r_pp, int is_c);
void free_block(Page **b,int size, int r_pp, int is_c );
void select_value(int r_pp, FILE *i_fp,int is_c ,int p_size,Opthions *args);
void join_value(FILE *o_fp,FILE *i_fp, int b_size, int p_size,int is_c, int r_pp);
void print_result(int is_c, Character *c, Guild *g, Opthions * args);
int join_select(Character *c, Guild *g, char *attr, char *value, int is_c, int flag, Opthions *args);
int is_character(FILE *fp);
int is_char(char *name);
#endif
