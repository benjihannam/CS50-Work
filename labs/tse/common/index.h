/* 
 * index - a set of functions for creating, saving, loading the index
 * 
 * The index is keyed by words (strings) and stores counter sets; 
 * each counter set represents the set of documents where that word 
 * appeared, and the number of occurrences of that word in each document.
 * 
 * Benji Hannam, Summer 2016
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "index.h"
#include "../lib/hashtable/hashtable.h"
#include "../lib/hashtable/jhash.h"
#include "../lib/set/set.h"
#include "../lib/counter/counters.h"

/**************** global types ****************/

// The index is actually a hashtable. 
typedef hashtable_t index_t;

/********** prototypes *********/

/**************** index_new() ****************/
/* 
 * Creates the new index structure
 */
index_t *index_new(int num_slots, void (itemdelete)(void *data));

/**************** index_insert() ****************/
/* 
 * inserts a new counter into the hashtable
 */
bool index_insert(index_t *index, char *key, counters_t *counters);

/**************** index_find() ****************/
/* 
 * looks to see if a counters already exists for that word, returns the counters if it does, 
 * otherwise returns NULL
 */
counters_t *index_find(index_t *index, char *word);

/**************** index_print() ****************/
/* 
 * Prints the word and the counters for that word to a file called filename.
 */
void index_print(void *fp, const char *key, void *ctrs);

/**************** index_save() ****************/
/* 
 * Prints the index to a file given by index_filename
 */
void index_save(index_t *index, char* index_filename);

/**************** index_deletes() ****************/
/* 
 * deletes the index
 */
void index_delete(index_t *index);

/**************** print_counters() ****************/
/* 
 * prints the output of the counter
 */
void print_counters(void *fp, const int key, int count);

/******************* index_load() **************************/
/* 
 * Create an index from a file that has the words, the docIDs and their counts
 */
void index_load(index_t *index, char* filename);

#endif // __INDEX_H
