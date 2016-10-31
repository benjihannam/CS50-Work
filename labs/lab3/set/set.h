/* 
 * SET.h - header file for set
 *
 * Benji Hannam, July 2016
 */

#ifndef __SET_H
#define __SET_H

/**************** global types ****************/
typedef struct set set_t;  // opaque to users of the module

/**************** functions ****************/

/* Add a new counter if the key doesn't exist, if it does then increment the count of that counter 
 * 
 */
set_t *set_new(void);

/* 
 * Looks for the pair with key, returns the data if its there, returns NULL if it isn't
 */
void *set_find(set_t *set, char *key);

/* inserts a new string/data pair into the set, returns false if the key is already there and true if the pair is added.
 * 
 */
bool set_insert(set_t *set, char *key, void *data);

#endif // __SET_H
