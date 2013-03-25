/***************************************************************************
 *
 *   File        : sl_list.c
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
#include "sl_list.h"

/* create an empty list */
list_t         *
list_create(void)
{
   fprintf(stderr, "list_create() please implement me!\n");
   exit(-1);
}

/* free all memory associated with list */
void
list_free(list_t * list)
{
   fprintf(stderr, "list_free() please implement me!\n");
   exit(-1);
}

/* Insert items in an order defined by cmp_fptr */
list_t         *
list_insert_inorder(list_t * list, void *value,
		    int (*cmp_fptr) (const void *, const void *))
{
   fprintf(stderr, "list_insert_inorder() please implement me!\n");
   exit(-1);
}

/* Insertion sort for a generic link list */
list_t         *
list_sort(list_t * list, int (*cmp_fptr) (const void *, const void *))
{
   fprintf(stderr, "list_sort() please implement me!\n");
   exit(-1);
}
