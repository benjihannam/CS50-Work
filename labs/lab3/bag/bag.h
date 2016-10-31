/* 
 * bag.h - header file for bag
 *
 * Benji Hannam, July 2016
 */

#ifndef __BAG_H
#define __BAG_H

/**************** global types ****************/
typedef struct bag bag_t;  // opaque to users of the module

/**************** functions ****************/

/* Create a new bag 
 * 
 */
bag_t *bag_new(void);

/* Insert an item into the bag
 * 
 */
void bag_insert(bag_t *bag, void *data);

/* Pull an item out of the bag and return it to the user
 * 
 */
void *bag_extract(bag_t *bag);

#endif // __BAG_H
