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

#endif // __COUNTERS_H
