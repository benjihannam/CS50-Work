/* 
 * index - a set of functions for creating, saving, loading the index
 * 
 * See index.h for interface descriptions.
 * 
 * Benji Hannam, August 2016
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "index.h"
#include "../lib/hashtable/hashtable.h"
#include "../lib/hashtable/jhash.h"
#include "../lib/set/set.h"
#include "../lib/counter/counters.h"


/*************** global types *******************/
typedef hashtable_t index_t;

/**************** index_new() ****************/
/* 
 * Creates the new index structure
 */
index_t *index_new(int num_slots, void (itemdelete)(void *data)){
	index_t *new = hashtable_new(num_slots, itemdelete);
	return new;
}

/**************** index_insert() ****************/
/* 
 * inserts a new counter into the hashtable
 */
bool index_insert(index_t *index, char *key, counters_t *counters){
		
		return hashtable_insert(index, key, counters);
}

/**************** index_find() ****************/
/* 
 * looks to see if a counters already exists for that word, returns the counters if it does, 
 * otherwise returns NULL
 */
counters_t *index_find(index_t *index, char *word){
	
	return (counters_t*)hashtable_find(index, word);
}

/**************** index_print() ****************/
/* 
 * Prints the word and the counters for that word to a file called filename.
 */
void index_print(void *fp, const char *key, void *ctrs){

	fprintf((FILE*)fp, "%s ", key);

	counters_iterate((counters_t*)ctrs, print_counters, fp);
	fprintf((FILE*)fp, "\n");
}

/**************** index_save() ****************/
/* 
 * Prints the index to a file given by index_filename
 */
void index_save(index_t *index, char* index_filename){

	FILE *fp;
	//open the file
	fp = fopen(index_filename, "w");
	if (fp == NULL){
		printf("Error opening file\n");
		exit(3);
	}

	//iterate through the hashtable
	hashtable_iterate(index, index_print, fp);
	fclose(fp);

}

/**************** index_delete() ****************/
/* 
 * deletes the index
 */
void index_delete(index_t *index){
	hashtable_delete(index);
}

/**************** print_counters() ****************/
/* 
 * prints the output of the counter
 */
void print_counters(void *fp, const int key, int count){

	fprintf((FILE*)fp, "%d %d ", key, count);
	
}

/******************* index_load() **************************/
/* 
 * Create an index from a file that has the words, the docIDs and their counts
 */
void index_load(index_t *index, char* filename){

	FILE *fp;
	//open the file
	fp = fopen(filename, "r");
	if (fp == NULL){
		printf("Error opening file\n");
		exit(3);
	}

	//variables
	const char s[2] = " ";
	char line[1000];
	char *token;
	bool first = true, docID = false, word_count = false;
	char word[100];
	int ID, count;
	counters_t *new_counter;

	//get the line
	while(fgets(line, 1000, fp) != NULL){
		//split it by spaces
		token = strtok(line, s);
		//reset first to true
		first = true;
		//create a new counters to hold the words
		new_counter = counters_new();

		while (token != NULL){
			
			if (first){
				//copy token into word
				strcpy(word,token);
				//next loop will get a docID
				first = false;
				docID = true;
				word_count = false;

			}
			else if(docID){
				//turn token into an int and store in ID
				ID = atoi(token);	
				//next loop will get word_count
				docID = false;
				word_count = true;
				first = false;
			}
			else if(word_count){
				//turn token into an int and store in count
				count = atoi(token);
				//if there are more than the next one is a docID
				docID = true;
				word_count = false;
				first = false;
				//also insert the id/count into the counters
				counters_set(new_counter, ID, count);
			}
			//move onto next part of the line
			token = strtok(NULL, s);
		}
		//insert into hashtable
		index_insert(index, word, new_counter);
	}
	printf("Index Loaded.\n");
	fclose(fp);
}









