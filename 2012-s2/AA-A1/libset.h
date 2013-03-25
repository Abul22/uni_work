#ifndef _LIBSET_H_
#define _LIBSET_H_

#ifdef __cplusplus
extern          "C" {
#endif				/** cplusplus **/

#include "libutil.h"

#define DEFAULT_SET_SIZE        128

/** Basic Set Type **/
typedef struct {
	uint64_t       *data;	/* set elements */
	uint64_t		size;   /* size of the data memory block */
	uint64_t        items;	/* number of items in the set */
	uint64_t        n_max;	/* value of the maximum item */
	int             lock;	/* set state */
} set_t;

/** counter operations **/
void            reset_cmp_cnt();
uint64_t		get_cmp_cnt();

/** Basic Set Operations **/
int             set_insert(set_t * set,uint64_t item);
set_t*          set_create(uint64_t size);
void            set_destroy(set_t * set);
int             set_freeze(set_t * set);

/** Search Operations for array sets **/
int64_t         linear_search(uint64_t *set, uint64_t len, uint64_t key);
int64_t         binary_search(uint64_t *set, uint64_t len, uint64_t key);
int64_t         interpolation_search(uint64_t *set, uint64_t len, uint64_t key);
int64_t         exponential_search(uint64_t *set, uint64_t len, uint64_t key);
int64_t         block_search(uint64_t *set, uint64_t len, uint64_t key, uint64_t b);
/** the function build by myself **/
void            read_data(set_t *pset,char *pname);
#ifdef __cplusplus
}
#endif /** cplusplus **/

#endif /** _LIBSET_H_ **/
