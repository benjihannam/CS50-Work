/* 
 * counters.h - header file for counters
 *
 * Benji Hannam, July 2016
 */

#ifndef __COUNTERS_H
#define __COUNTERS_H

/**************** global types ****************/
typedef struct counters counters_t;  // opaque to users of the module

/**************** functions ****************/
/*Creates a new set of counters
*
*/
counters_t *counters_new(void);

/* Add a new counter if the key doesn't exist, if it does then increment the count of that counter 
 * 
 */
void counters_add(counters_t *ctrs, int key);

/* 
 * Get the current value for a key, returns 0 if the key is not found
 */
int counters_get(counters_t *ctrs, int key);

/* Delete the counters
 * 
 */
void counters_delete(counters_t *ctrs);

/**************** counters_set() ****************/
/* Set value of the counter indicated by key.
 * If the key does not yet exist, create a counter for it
 * and initialize its counter value to 'count'.
 * NULL counters is ignored.
 */
void counters_set(counters_t *ctrs, int key, int count);

/**************** counters_iterate() ****************/
/* Iterate over all counters in the set (in undefined order):
 * call itemfunc for each item, with (arg, key, count).
 */
void counters_iterate(counters_t *ctrs, void (*itemfunc)(void *arg, const int key, int count), void *arg);

/* 
 * Prints out the counters
 */
void counters_intersect_iterate(counters_t *ctrs, void (*itemfunc)(void *arg, const int key, int *count), void *arg);

#endif // __COUNTERS_H
