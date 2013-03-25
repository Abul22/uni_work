/***************************************************************************
 *
 *   File        : libintersect.c
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
#include "libintersect.h"

int
linear_fsearch(int *set, int len, int key)
{
   fprintf(stderr, "linear_fsearch() please implement me!\n");
   exit(-1);
}

int
binary_fsearch(int *set, int len, int key)
{
   fprintf(stderr, "binary_fsearch() please implement me!\n");
   exit(-1);
}

int
interpolation_fsearch(int *set, int len, int key)
{
   fprintf(stderr, "interpolation_fsearch() please implement me!\n");
   exit(-1);
}

int
exponential_fsearch(int *set, int len, int key)
{
   fprintf(stderr, "exponential_fsearch() please implement me!\n");
   exit(-1);
}

set_t          *
svs(list_t * list, int (*srch_alg) (int *, int, int))
{
   fprintf(stderr, "svs() please implement me!\n");
   exit(-1);
}

set_t          *
sequential(list_t * list, int (*srch_alg) (int *, int, int))
{
   fprintf(stderr, "sequential() please implement me!\n");
   exit(-1);
}

set_t          *
adaptive(list_t * list, int (*srch_alg) (int *, int, int))
{
   fprintf(stderr, "adaptive() please implement me!\n");
   exit(-1);
}
