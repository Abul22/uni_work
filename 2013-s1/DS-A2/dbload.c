/***************************************************************************
 *
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
#include "util.h"
int main(int argc,  char *argv[])
{
  char out_file[MAX_LEN] = {0};
  char line[MAX_LEN+1];
  FILE *i_fp,*o_fp;
  int is_c,r_pp,r_read = 0;
  /* init the argument  */
  Opthions *args = arg_load(argc,argv);
  check_file(args->i_file,&i_fp,"r");
  is_c = is_character(i_fp);
  /* set the out_file's name to relation.pagesize */
  strcpy(out_file,args->dir);
  if (is_c)
    strcat(out_file,C_STRING);
  else
    strcat(out_file,G_STRING);
  strcat(out_file,args->p_size);
  args->o_file = out_file;
  check_file(args->o_file,&o_fp,"wb");
  /* Get the records per page - pagesize / recordsize  */
  r_pp = ceil(atoi(args->p_size)/RECORD_SIZE);
  Character *c_buffer, *c_start ;
  Guild *g_buffer , *g_start ;
  c_buffer = safe_malloc(atoi(args->p_size));
  c_start = c_buffer;
  g_buffer = safe_malloc(atoi(args->p_size));
  g_start = g_buffer;
  while(fgets(line,MAX_LEN+1,i_fp) != NULL){
    line[strlen(line)-1] = '\0';
    /* read a record line to a struct  */
    if(is_c) {
      memcpy(c_buffer,create_record(line,is_c),RECORD_SIZE);
      c_buffer++;
    }
    else{
      memcpy(g_buffer,create_record(line,is_c),RECORD_SIZE);
      g_buffer++;
    }
    r_read++;
    if (r_read == r_pp) {
      /* if read enough a page write it out  */
      if (is_c) {
        fwrite(c_start,1,atoi(args->p_size),o_fp);
        memset(c_start,0,atoi(args->p_size));
        c_buffer = c_start;
      }else{
        fwrite(g_start,1,atoi(args->p_size),o_fp);
        memset(c_start,0,atoi(args->p_size));
        g_buffer = g_start;
      }
      r_read = 0;
    }
  }
  if(r_read != 0){
    /* if have not read enough a page write it to a new page */
    if (is_c) {
      fwrite(c_start,1,atoi(args->p_size),o_fp);
    }else{
      fwrite(g_start,1,atoi(args->p_size),o_fp);
    }
  }
  if (is_c) c_buffer = c_start;
  else g_buffer = g_start;
  /* clean up  */
  free(c_buffer);
  free(g_buffer);
  free(args);
  /* Close the file */
  fclose(i_fp);
  fclose(o_fp);

  return 0;
}

