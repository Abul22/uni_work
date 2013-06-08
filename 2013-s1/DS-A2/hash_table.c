/***************************************************************************
 *
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/

#include "hash_table.h"

/*
 * hash_int()
 * init the hashtable with the defalut size - 250
 */
Hashtable *hash_init(int size){
  int i;
  Hashtable *ht = safe_malloc(sizeof(Hashtable));
  ht->table = safe_calloc(size,sizeof(Entry*));
  ht->size =size;

  for (i = 0; i < size; i++)
    ht->table[i] = NULL;
  return ht;
}

/*
 * hash_value()
 * the hash function for Hash table
 */
int hash_value(int key, int size)
{
  return ((MULT * key + SEED) % PRIME) % size;
}

/*
 * hash_insert()
 * Insert the entry to hash table
 */
void hash_insert(Hashtable *ht, int key, void *value)
{
  int h = hash_value(key,ht->size);
  Entry *e = safe_malloc(sizeof(Entry));
  if (ht->table[h] == NULL) {
    e->next = NULL;
    ht->table[h] = e;
  }else{
    e->next = ht->table[h];
    ht->table[h] = e;
  }
  e->key = key;
  e->value = value;
}

/*
 * hash_block()
 * hash the block
 */
void hash_block(Hashtable *ht,Page **block,int size,int r_pp,int is_c)
{
  int i,j;
  for (i = 0; i < size; i++) {
    if(!block[i])
      break;
    for (j = 0; j < r_pp; j++) {
      if(is_c){
        if(block[i]->c[j] != NULL)
          hash_insert(ht,block[i]->c[j]->guild_id,block[i]->c[j]);
      }
      else{
        if(block[i]->g[j] != NULL)
          hash_insert(ht,block[i]->g[j]->guild_id,block[i]->g[j]);
      }
    }
  }
}

/*
 * hash_find()
 * find the Entry based on the key
 */
Entry *hash_find(Hashtable *ht, int key)
{
  int hash = hash_value(key,ht->size);
  return ht->table[hash];
}

/*
 * hash_clear()
 * Clear the Entry in the hash table
 */
void hash_clear(Hashtable *ht)
{
  int i;
  for (i = 0; i < ht->size; i++) {
    if(ht->table[i])
      entry_free(ht->table[i]);
  }
  for(i = 0; i < ht->size;i++)
    ht->table[i] = NULL;
}

/*
 * entry_free()
 * free the entry
 */
void entry_free(Entry *e)
{
  if(e->next != NULL)
    entry_free(e);
  free(e);
}

/*
 * hash_free()
 * free the hash table
 */
void hash_free(Hashtable *ht)
{
  free(ht->table);
  free(ht);
}
