/***************************************************************************
 *
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "util.h"

#define  HASH_SIZE      250

#define PRIME           49157
#define MULT            24953
#define SEED            12289

typedef struct entry{
  int key;
  void *value;
  struct entry *next;
}Entry;

typedef struct {
  int size;
  Entry **table;
}Hashtable;


Hashtable *hash_init(int size);
void hash(int key, int size);
void hash_block(Hashtable *ht,Page **b,int size,int r_pp,int is_c);
Entry *hash_find(Hashtable *ht, int key);
void entry_free(Entry *e);
void hash_clear(Hashtable *ht);
void hash_free(Hashtable *ht);
#endif
