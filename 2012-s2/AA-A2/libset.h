/***************************************************************************
 *
 *   File        : libset.h
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
/*** Miscelaneous Utility Functions ***/
#ifndef _LIBSET_H_
#define _LIBSET_H_

#ifdef __cplusplus
extern          "C" {
#endif				/** cplusplus **/

#include "libutil.h"

   /** Basic Set Type **/
typedef struct {
   int   *data; /* set elements */
   int   size; /*Size of the data memory  
                 block*/
   int   items;
   int   n_max; /* Value of maximum item. */
   int   lock;  /* set state */
   int   finger;
}set_t;

   /** Basic Set Operations **/
   int   set_insert(set_t * set, int item);
   set_t *set_create(int size);
   void set_destroy(set_t * set);
   int set_freeze(set_t * set);

	/** Search Operations for array sets **/
   int   linear_search(int *set, int len, int key);
   int   binary_search(int *set, int len, int key);
   int   interpolation_search(int *set, int len, int key);
   int   exponential_search(int *set, int len, int key);

#ifdef __cplusplus
}
#endif				/** cplusplus **/

#endif				/** _PUTIL_H_ **/
