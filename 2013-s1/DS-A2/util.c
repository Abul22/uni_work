/***************************************************************************
 *
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
#include "util.h"
/*
 * print_usage()
 * print out the usage if something wrong
 */
void print_usage(const char *program)
{
  fprintf(stderr, "Usage:%s [-p pagesize] [-d directory] [-b buffers] [-t] <query_plan> \n",program);
  fprintf(stderr, "<query_plan> ::= <from_cmd> [query_step]\n");
  fprintf(stderr, "<query_step> ::= <select_cmd> | <join_cmd>\n");
  fprintf(stderr, "<from_cmd> ::= from <relation_name> \n");
  fprintf(stderr, "<select_cmd> ::= select <attribute_name> <value>\n");
  fprintf(stderr, "<join_cmd> ::= select <relation_name>\n");
  fprintf(stderr, "<relation_name> ::= characters | guilds\n");
  fprintf(stderr, "<attribute_name ::= name | species | class | id | guild_id | g_name\n");
  fprintf(stderr, "<value> ::= <string> | integer \n");
  fprintf(stderr, "./dbquery from characters select name Inepsan join guilds\n");
  fprintf(stderr, "Warning: Buffers shuld greate than 3!\n");
}

/*
 * safe_malloc()
 * safely malloc the memory
 */
void *safe_malloc(unsigned int n)
{
  void *p = malloc(n);
  if (!p) {
    fprintf(stderr, "safe_malloc: not enough memory\n");
    exit(1);
  }
  return p;
}

/*
 * safe_calloc()
 * safely calloc the memory
 */
void *safe_calloc(unsigned int n,unsigned int size)
{
  void *p = calloc(n,size);
  if (!p) {
    fprintf(stderr, "safe_calloc: not enough memory\n");
    exit(1);
  }
  return p;
}

/*
 * arg_init()
 * initilize the argument pass in to a Opthions struct
 * init with the default value
 */
Opthions *arg_init()
{
  Opthions *op = safe_malloc(sizeof(Opthions));
  op->p_size = "1024"; /* defalult page size */
  op->buffers = 3; /* defalult buffer size */
  op->flag = 0; /* defalult flag is 0 */
  op->dir = "./"; /* defalult dir location */
  op->o_file = NULL;
  op->i_file = NULL;
  op->a_name = NULL;
  op->value = NULL;
  op->project = NULL;
  return op;
}

/*
 * check_file()
 * check the file, safely read or write the file
 */
void  check_file(char *name,FILE **fp, const char *m){
   if((*fp = fopen(name,m)) == NULL){
     fprintf(stderr, "%s can not open!\n",name);
     exit(EXIT_FAILURE);
   }
}

/*
 * arg_load()
 * For dbload, use getopt() to set up the flags
 */
Opthions *arg_load(int argc, char** argv)
{
  int opt;
  Opthions *args = arg_init();
  while ((opt = getopt(argc,argv,"p:d")) != -1) {
    switch (opt) {
      case 'p':
        args->p_size = optarg;
        break;
      case 'd':
        args->dir = optarg;
        break;
      default:
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  args->i_file = argv[optind];
  return args;
}

/*
 * arg_query()
 * For dbquery, use getopt() to set up the flags
 */
Opthions *arg_query(int argc, char** argv)
{
  int opt,i, is_j;
  Opthions *args = arg_init();
  while ((opt = getopt(argc,argv,"p:d:b:t")) != -1) {
    switch (opt) {
      case 'p':
        args->p_size = optarg;
        break;
      case 'd':
        args->dir = optarg;
        break;
      case 'b':
        args->buffers = atoi(optarg);
        break;
      case 't':
        args->flag = 1;
        break;
      default:
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  i = optind;
  if (i+1 > argc) {
    print_usage(argv[0]);
    exit(EXIT_FAILURE);
  }else if(strcmp(argv[i],"from") == 0){
    args->o_file = argv[i+1];
    i += 2;
  }else{
    print_usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  while(i < argc){
    if (strcmp(argv[i],"join") == 0 && (i + 1 < argc)) {
      args->i_file = argv[i+1];
      i += 2;
    }else if(strcmp(argv[i],"select") == 0 && (i + 2 < argc)){
      args->a_name = argv[i+1];
      args->value = argv[i+2];
      i += 3;
    }
    else if (strcmp(argv[i],"project") == 0 && (i + 1 < argc)){
      args->project = argv[i+1];
      i += 2;
    }
    else{
      print_usage(argv[0]);
      exit(EXIT_FAILURE);
    }
  }
  is_j = (strcmp(argv[optind+2],"join") == 0);

  return args;
}

/*
 * is_character()
 * check the file is Character or Guild
 */
int is_character(FILE *fp)
{
  int n = 0;
  char line[MAX_LEN];
  fgets(line,MAX_LEN+1,fp);
  strtok(line,",");
  while(strtok(NULL,",")) n++;
  fseek(fp,0,SEEK_SET);
  return n>2;
}

/*
 * is_char()
 * check the file is Character.pagesize or Guild.pagesize
 */
int is_char(char *name){
   char r_name[C_NAME_LEN];
   strcpy(r_name,strtok(name,"."));
   if (strcmp(name,"characters")==0)
     return 1;
   else if(strcmp(name,"guilds")==0)
     return 0;
   else
      exit(EXIT_FAILURE);
}

/*
 * create_record()
 * read the line and create records for Character or Guild
 */
void *create_record(char *line, int is_c)
{
  if (is_c) {
    Character *c = safe_calloc(1,sizeof(Character));
    strcpy(c->name, strtok(line,","));
    c->species = atoi(strtok(NULL,","));
    c->class = atoi(strtok(NULL,","));
    c->id = atoi(strtok(NULL,","));
    c->guild_id = atoi(strtok(NULL,","));
    return c;
  }
  else{
    Guild *g = safe_calloc(1,sizeof(Guild));
    g->guild_id = atoi(strtok(line,","));
    strcpy(g->g_name, strtok(NULL,","));
    return g;
  }
}

/*
 * free_page()
 * free the page
 */
void free_page(Page *page, int r_pp, int is_c)
{
  int i;
  for (i = 0; i < r_pp; i++) {
    if(is_c) free(page->c[i]);
    else free(page->g[i]);
  }
    if(is_c) free(page->c);
    else free(page->g);
  free(page);
}

/*
 * read_page()
 * read the heap file and set the record to
 * Character and Guild
 */
Page *read_page(int r_pp, FILE *fp,int is_c,int p_size)
{
  int i,size = r_pp * BUFFER_SIZE;
  Character *c_buffer = safe_malloc(size);
  Character *c_start = c_buffer;
  Guild *g_buffer = safe_malloc(size);
  Guild *g_start = g_buffer;
  if(is_c) fread(c_buffer,1,p_size,fp);
  else fread(g_buffer,1,p_size,fp);
  Page *p = safe_malloc(sizeof(Page));
  if (is_c) p->c = safe_calloc(r_pp,sizeof(Character*));
  else p->g = safe_calloc(r_pp, sizeof(Guild*));
  for (i = 0; i < r_pp; i++) {
    if (is_c) {
      Character *c = safe_malloc(sizeof(Character));
      memcpy(c,c_buffer,RECORD_SIZE);
      c_buffer++;
      if(strlen(c->name) != 0)
        p->c[i] = c;
      else{
        p->c[i] = NULL;
        break;
      }
    }else{
      Guild *g = safe_malloc(sizeof(Guild));
      memcpy(g,g_buffer,RECORD_SIZE);
      g_buffer++;
      if(strlen(g->g_name) != 0)
        p->g[i] = g;
      else{
        p->g[i] = NULL;
        break;
      }
    }
  }
  g_buffer = g_start;
  c_buffer = c_start;
  free(c_buffer);
  free(g_buffer);
  return p;
}

/*
 * select_value()
 * Select function done for
 * 1.2 Select from relation using heap file
 */
void select_value(int r_pp, FILE *fp,int is_c ,int p_size,Opthions *args)
{
  int i,match;
  while (!feof(fp)) {
    Page *page = read_page(r_pp,fp,is_c,p_size);
    for (i = 0; i < r_pp; i++) {
      if(is_c){
        if(page->c[i] != NULL){
          if(strcmp(args->a_name,"name") == 0){
            if (strcmp(page->c[i]->name,args->value) == 0 ) {
              fprintf(stderr, "%s,%d,%d,%d,%d\n",page->c[i]->name,page->c[i]->species,
                  page->c[i]->class,page->c[i]->id,page->c[i]->guild_id);
              match++;
            }
          }
          else if(strcmp(args->a_name,"species")==0)
          {
            if(page->c[i]->species == atoi(args->value)){
              fprintf(stderr, "%s,%d,%d,%d,%d\n",page->c[i]->name,page->c[i]->species,
                  page->c[i]->class,page->c[i]->id,page->c[i]->guild_id);
              match++;
            }
          }
          else if(strcmp(args->a_name,"class")==0)
          {
            if(page->c[i]->class == atoi(args->value)){
              fprintf(stderr, "%s,%d,%d,%d,%d\n",page->c[i]->name,page->c[i]->species,
                  page->c[i]->class,page->c[i]->id,page->c[i]->guild_id);
              match++;
            }
          }
          else if(strcmp(args->a_name,"id")==0)
          {
            if(page->c[i]->id == atoi(args->value)){
              fprintf(stderr, "%s,%d,%d,%d,%d\n",page->c[i]->name,page->c[i]->species,
                  page->c[i]->class,page->c[i]->id,page->c[i]->guild_id);
              match++;
            }
          }
          else if(strcmp(args->a_name,"guild_id")==0)
          {
            if(page->c[i]->guild_id == atoi(args->value)){
              fprintf(stderr, "%s,%d,%d,%d,%d\n",page->c[i]->name,page->c[i]->species,
                  page->c[i]->class,page->c[i]->id,page->c[i]->guild_id);
              match++;
            }
          }else{
            fprintf(stderr, "The attritube %s is not correct\n",args->a_name);
            exit(EXIT_FAILURE);
          }
        }
      }
      else{
        if(page->g[i] != NULL){
          if(strcmp(args->a_name,"g_name") == 0){
            if (strcmp(page->g[i]->g_name,args->value) == 0 ) {
              fprintf(stderr, "%d,%s\n",page->g[i]->guild_id,page->g[i]->g_name);
              match++;
            }
          }else if(strcmp(args->a_name,"guild_id") == 0){
            if (page->g[i]->guild_id == atoi(args->value)) {
              fprintf(stderr, "%d,%s\n",page->g[i]->guild_id,page->g[i]->g_name);
              match++;
            }
          }else{
            fprintf(stderr, "The attritube %s is not correct\n",args->a_name);
            exit(EXIT_FAILURE);
          }
        }
      }
    }
    free_page(page,r_pp,is_c);
  }
  if(match == 0)
    fprintf(stderr, "Can not find %s's %s from %s\n",args->a_name,args->value,args->i_file);
}

/*
 * read_block()
 * read the data to the block besead on the block size
 */
Page** read_block(int size,FILE *fp,int is_c, int p_size)
{
  int i;
  int r_pp = ceil(p_size/RECORD_SIZE);
  Page **block = safe_calloc(size,sizeof(Page*));
  for (i = 0; i < size; i++) {
    block[i] = read_page(r_pp,fp,is_c,p_size);
    if(!block[i])
      return NULL;
  }
  return block;
}

/*
 * free_block()
 * safely free the block
 */
void free_block(Page **b,int size, int r_pp, int is_c )
{
  int i;
  for (i = 0; i < size; i++) {
    if(b[i])
      free_page(b[i],r_pp,is_c);
  }
  free(b);
}

/*
 * join_select()
 * select the value after nested loops join
 */
int join_select(Character *c, Guild *g, char *attr, char *value, int is_c, int flag, Opthions *args)
{
  int find = 0 ;
  if (strcmp(attr,"name") == 0 && strcmp(c->name,value) == 0){
   if(!flag) print_result(is_c,c,g,args);
    find++;
  }
  else if(strcmp(attr,"g_name") == 0 && strcmp(g->g_name,value) == 0){
   if(!flag) print_result(is_c,c,g,args);
    find++;
  }
  else if(strcmp(attr,"guild_id") == 0 && g->guild_id == atoi(value)){
   if(!flag) print_result(is_c,c,g,args);
    find++;
  }
  else if(strcmp(attr,"id") == 0 && c->id == atoi(value)){
   if(!flag) print_result(is_c,c,g,args);
    find++;
  }
  else if(strcmp(attr,"class") == 0 && c->class == atoi(value)){
   if(!flag) print_result(is_c,c,g,args);
    find++;
  }
  else if(strcmp(attr,"species") == 0 && c->species == atoi(value)){
   if(!flag) print_result(is_c,c,g,args);
    find++;
  }

  return find;
}

/*
 * print_result()
 * print the result if the flag is require
 */
void print_result(int is_c, Character *c, Guild *g, Opthions * args)
{
  if (args->project) {
    if (strcmp(args->project,"name") == 0)
      fprintf(stderr, "%s\n",c->name);
    else if (strcmp(args->project,"species") == 0)
      fprintf(stderr, "%d\n",c->species);
    else if (strcmp(args->project,"id") == 0)
      fprintf(stderr, "%d\n",c->id);
    else if (strcmp(args->project,"class") == 0)
      fprintf(stderr, "%d\n",c->class);
    else if (strcmp(args->project,"guild_id") == 0)
      fprintf(stderr, "%d\n",c->guild_id);
    else if (strcmp(args->project,"g_name") == 0)
      fprintf(stderr, "%s\n",g->g_name);
    else{
      fprintf(stderr, "Wrong input wiht project, please input these arrtirbute\n");
      fprintf(stderr, "name | species | id | class | guild_id | g_name\n");
    }
  }
  else {
    if(is_c){
      fprintf(stderr, "%d,%s,%d,%d,%d,%s\n",c->guild_id,c->name,c->species,
          c->class,c->id,g->g_name);
    }else{
      fprintf(stderr, "%d,%s,%s,%d,%d,%d\n",g->guild_id,g->g_name,c->name,
          c->species, c->class,c->id);
    }
  }
}
