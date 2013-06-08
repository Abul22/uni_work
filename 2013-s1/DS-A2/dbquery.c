/***************************************************************************
 *
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
#include "util.h"
#include "hash_table.h"
int main(int argc,  char *argv[])
{
  FILE *i_fp,*o_fp;
  int is_c,r_pp,b_size,p_size,i,find,matches = 0;
  struct timespec start,stop;
  /* init the argument  */
  Opthions *args = arg_query(argc,argv);
  /* Get the records per page - pagesize / recordsize  */
  r_pp = ceil(atoi(args->p_size)/RECORD_SIZE);
  /* remove the inner and outer files of buffers */
  b_size = args->buffers - 2;
  p_size = atoi(args->p_size);
  check_file(args->i_file,&i_fp,"r");
  if(args->o_file){
    check_file(args->o_file,&o_fp,"r");
    is_c = is_char(args->o_file);
  }
  else{
    /* without join require, run select function and exit  */
    is_c = is_char(args->i_file);
    select_value(r_pp,i_fp,is_c,atoi(args->p_size),args);
    return EXIT_SUCCESS;
  }
  /* Get the start point's time  */
  clock_gettime(CLOCK_REALTIME,&start);
  while (!feof(o_fp)) {
    /* init the hash table  */
    Hashtable * ht = hash_init(HASH_SIZE);
    /* set up the block */
    Page **block = read_block(b_size,o_fp,is_c, p_size);
    hash_block(ht,block,b_size,r_pp,is_c);
    while(!feof(i_fp)){
      /* read the inner file to page */
      Page *p = read_page(r_pp,i_fp,!is_c,p_size);
      for (i = 0; i < r_pp; i++) {
        Character *c = NULL;
        Guild *g = NULL;
        Entry *e = NULL;
        /* find the entry based on the guild_id */
        if (is_c) {
          if(!p->g[i])
            continue;
          g = p->g[i];
          e = hash_find(ht,g->guild_id);
        }else{
          if(!p->c[i])
            continue;
          c = p->c[i];
          e = hash_find(ht,c->guild_id);
        }
        /* If find the Entry check the guild_id */
        while (e != NULL) {
          if(is_c) c = (Character*)e->value;
          else g = (Guild*)e->value;
          if(g->guild_id == c->guild_id){
            if (args->a_name) {
              /* if need select the attribute value, run the select after join  */
              find = join_select(c,g,args->a_name,args->value,is_c,args->flag,args);
              if (find) matches++;
            }
            else{
              /* if not need select, just finish join */
                if(!args->flag) print_result(is_c,c,g,args);
              if(matches < RECORDS ) matches++;
            }
          }
          /* using chaining, so get next possible match */
          e = e->next;
        }
      }
      /* deallocate page */
      free_page(p,r_pp,!is_c);
    }
    /* rewind inner file */
    fseek(i_fp,0,SEEK_SET);

    /* deallocate block */
    free_block(block,b_size,r_pp,is_c);
    hash_free(ht);
  }
  clock_gettime(CLOCK_REALTIME,&stop);
  /* Get the time cost in join function  */
  double time = (stop.tv_nsec-start.tv_nsec)/TIMESECORD;
  if(matches == 0 && args->a_name){
    fprintf(stderr, "Can not find %s's %s from %s\n",args->a_name,args->value,args->i_file);
  }
  fprintf(stderr, "Number of tuples: %d \ntime: %.2lf \n",matches, time);
  /* clearn up and exit  */
  free(args);
  fclose(i_fp);
  fclose(o_fp);
  return 0;
}

