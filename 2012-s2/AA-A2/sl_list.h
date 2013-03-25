/***************************************************************************
 *
 *   File        : sl_list.h
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
/*** Utility Types ***/
#ifndef _SL_LIST_H_
#define _SL_LIST_H_

#ifdef __cplusplus
extern          "C" {
#endif  /** cplusplus **/

#include "libutil.h"

   /* Link List data types */
typedef struct lnode lnode_t;

struct lnode {
   void *data;
   lnode_t *next;
};

typedef struct {
   lnode_t *head;
   lnode_t *tail;
   int list_len;
} list_t;

list_t *list_create(void);
void list_free(list_t * list);

/* Insert items in an order defined by cmp_fptr */
list_t *list_insert_inorder(list_t * list, void *value,
                            int (*cmp_fptr) (const void *, const void *));

	/* Insertion sort for a generic link list */
list_t *list_sort(list_t * list, int (*cmp_fptr) (const void *, const void *));


#ifdef __cplusplus
}
#endif				/** cplusplus **/

#endif				/** _SL_LIST_H_ **/
