/***************************************************************************
 *
 *   File        : libintersect.h
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
/*** Miscelaneous Utility Functions ***/
#ifndef _LIBINTERSECT_H_
#define _LIBINTERSECT_H_

#ifdef __cplusplus
extern          "C" {
#endif				/** cplusplus **/

#include "libutil.h"
#include "libset.h"
#include "sl_list.h"

   /** Finger Search Operations for array sets **/
int linear_fsearch(int *set, int len, int key);
int binary_fsearch(int *set, int len, int key);
int interpolation_fsearch(int *set, int len, int key);
int exponential_fsearch(int *set, int len, int key);

   /** Intersect Algorithms **/
set_t *svs(list_t * list, int (*srch_alg) (int *, int, int));
set_t *sequential(list_t * list, int (*srch_alg) (int *, int, int));
set_t *adaptive(list_t * list, int (*srch_alg) (int *, int, int));

#ifdef __cplusplus
}
#endif	/** cplusplus **/

#endif /** _LIBINTERSECT_H_ **/
