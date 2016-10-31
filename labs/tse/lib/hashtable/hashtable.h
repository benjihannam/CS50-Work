/* 
 * hashtable.h - header file for hashtable
 *
 * Benji Hannam, July 2016
 */

#ifndef _HASHTABLE_H
#define _HASHTABLE_H

/**************** global types ****************/
typedef struct hashtable hashtable_t;  // opaque to users of the module

/**************** functions ****************/

/* Create a new hashtable with a nunber of slots = num_slots
 * 
 */ 
 hashtable_t* hashtable_new(const int num_slots, void (*itemdelete)(void *data));

/* 
 * Returns the data for the given key, returns NULL if it isn't there
 */
void *hashtable_find(hashtable_t *ht, char *key);

/* inserts a new string/data pair into the hashtable, returns false if the key is already there and true if the pair is added. Returns NULL if an error occurs.
 * 
 */
bool hashtable_insert(hashtable_t *ht, char *key, void *data);

/* Deletes the hashtable
 * 
 */
void hashtable_delete(hashtable_t *ht);

/* Iterate over all items in hashtable (in undefined order):
 * call itemfunc for each item, with (arg, key, data).
 */
void hashtable_iterate(hashtable_t *ht, void (*itemfunc)(void *arg, const char *key,void *data), void *arg);

/* 
 * Print a hashtable of sets of counters
 */
void hashtable_counter_print(hashtable_t *ht, FILE *fp, void (*itemfunc)(FILE *fp, void *ctrs));

#endif // _ HASHTABLE_H
