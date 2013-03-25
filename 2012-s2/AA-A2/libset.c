/***************************************************************************
 *
 *   File        : libset.c
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
#include "libset.h"

/** Basic Set Operations **/

int
set_insert(set_t * set, int item)
{
   if (set->lock == TRUE) {
      fprintf(stderr, "Set is locked!\n");
      return (FALSE);
   }
   if ((set->items + 1) >= set->size) {
      set->size = set->size * 2;
      set->data = (int *) safe_realloc(set->data,set->size * sizeof(int));

      if (!set->data) {
         return (FALSE);
      }
   }
   set->data[set->items++] = item;
   set->n_max = item;

   return (TRUE);
}

set_t *
set_create(int size)
{
   set_t *rv = safe_malloc(sizeof(set_t));
   
   rv->data = safe_malloc(size * sizeof(int));

   rv->size = size;
   rv->items = 0;
   rv->n_max = 0;
   rv->lock = FALSE;
   rv->finger = -1;

   return (rv);
}

void
set_destroy(set_t * set)
{
   if (set->data) {
      free(set->data);
   }
   if (set) {
      free(set);
   }
}

int
set_freeze(set_t * set)
{
   set->data = safe_realloc(set->data, set->items * sizeof(int));
   set->lock = TRUE;
   if (!set->data) {
      return (FALSE);
   }
   return (TRUE);
}

/*
 * perform Algorithm 'LinearSearch' on the set and if 'key' is found in the
 * set return it's index otherwise return -1
 */
int
linear_search(int *set, int len, int key)
{
   int i;
   for (i = 0; i < len; i++) {
      if (set[i] == key)
         return i;
      else if (set[i] > key)
         return -1;
   }
   return (-1);
}

/*
 * perform Algorithm 'BinarySearch' on the set and if 'key' is found in the
 * set return it's index otherwise return -1
 */
int
binary_search(int *set, int len, int key)
{
   int l = 0;
   int r = len - 1;
   int m;

   while (l <= r) {
      m = (l + r) >> 1;
      if (set[m] == key)
        return m;
      else {
         if (key < set[m])
            r = m - 1;
         else
            l = m + 1;
      }
   }
   return (-1);
}

/*
 * perform Algortihm 'InterPolationSearch' on the set and if 'key' is found
 * in the set return it's index otherwise return -1
 */
int
interpolation_search(int *set, int len, int key)
{
   long long l = 0;
   long long r = len - 1;
   long long m;

   while (set[l] < key && set[r] >= key) {
      m = l + (((key - set[l]) * (r - l)) / (set[r] - set[l]));

      if (set[m] < key)
         l = m + 1;
      else {
         if (set[m] > key)
            r = m - 1;
         else
            return m;
      }
   }

   if (set[l] == key)
      return l;

	return (-1);
}


/*
 * perform Algorithm 'ExponentialSearch' on the set and if 'key' is found in
 * the set return it's index otherwise return -1
 */
int
exponential_search(int *set, int len, int key)
{
   int prev;
   int pos = 2;
   int res;

   while (pos < len && set[pos] < key) {
      prev = pos;
      pos = pos * 2;
   }

   if (pos > len - 1)
      pos = len - 1;

   if (set[pos] == key)
      return pos;

   res = binary_search(&set[prev], pos - prev, key);

   if (res == -1)
      return (-1);
   else
      return res + prev;

   return (-1);
}
