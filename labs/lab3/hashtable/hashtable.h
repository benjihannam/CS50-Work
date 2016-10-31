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
 hashtable_t* hashtable_new(const int num_slots);

/* 
 * Returns the data for the given key, returns NULL if it isn't there
 */
void *hashtable_find(hashtable_t *ht, char *key);

/* inserts a new string/data pair into the hashtable, returns false if the key is already there and true if the pair is added. Returns NULL if an error occurs.
 * 
 */
bool hashtable_insert(hashtable_t *ht, char *key, void *data);

#endif // _ HASHTABLE_H
