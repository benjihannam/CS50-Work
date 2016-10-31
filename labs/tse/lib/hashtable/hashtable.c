/* 
 * hashtable.h - a hashtable structure
 *
 * Includes a hash function, JenkinsHash() from the jhash.c file provided by the CS50 course. Delete function also done using example code from CS50.
 * Benji Hannam, July 2016
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../set/set.h"
#include "jhash.h"
#include "hashtable.h"


typedef struct hashtable{
 	int slot_number;
 	set_t* *table;
 	void (*itemdelete)(void*data);
} hashtable_t;

/**************** global functions ****************/

/**************** hashtable_new() ****************/
//Create a new hashtable with a nunber of slots = num_slots
hashtable_t* hashtable_new(const int num_slots, void (*itemdelete)(void *data)){

	//init hashtable
 	hashtable_t *ht = malloc(sizeof(hashtable_t));
	if (ht == NULL){
		printf("Error allocating memory to the hashtable.\n");
		exit(2);
	}

 	//init value of members
 	ht->slot_number = num_slots;
 	ht->itemdelete = itemdelete;

 	//init the table array
 	ht->table = malloc(num_slots * sizeof(set_t *));
 	if (ht->table == NULL){

 	}

 	//create sets
 	for (int i = 0; i < num_slots; i++){
 		set_t *s = set_new(itemdelete);
 		if (s == NULL){
 			//if it fails unwind and free all the previous ones 
 			for (int j=0; j < i; j++){
 				free(ht->table[j]);
 			} 
 			//as well as free ht and ht->table
 			free(ht->table);
 			free(ht);
 		}
 		else{
 			ht->table[i] = s;
 		}
 	}
 	return ht;
}

/**************** hashtable_find() ****************/
//Returns the data for the given key, returns NULL if it isn't there
void *hashtable_find(hashtable_t *ht, char *key){
 	//loop through each slot in the array
 	// for (int i = 0; i < ht->slot_number; i++){
 	// 	if ((set_find(ht->table[i], key)) != NULL){
 	// 		return set_find(ht->table[i], key);
 	// 	}
 	// }
 	int location = JenkinsHash(key, ht->slot_number);
 	if ((set_find(ht->table[location], key)) != NULL){
 			return set_find(ht->table[location], key);
 	}
 	else{
 		return NULL;
 }
}

/**************** hashtable_insert() ****************/
//inserts a new string/data pair into the hashtable, returns false if the key is already there and true if the pair is added. Returns NULL if an error occurs.
bool hashtable_insert(hashtable_t *ht, char *key, void *data){
 	if (ht == NULL){
 		return false;
 	}
 	int location = JenkinsHash(key, ht->slot_number);
 	if (hashtable_find(ht, key) != NULL){
 		return false;
 	}
 	else{
 		if (set_insert(ht->table[location], key, data)){
 			return true;
 		}
 		else{
 			return false;
 		}
 	}
}

/**************** hashtable_delete() ****************/
void hashtable_delete(hashtable_t *ht){

	if (ht != NULL){
		//go through each spot in the array
		for (int i = 0; i < ht->slot_number; i++){
			set_delete(ht->table[i]);
		}
		
		if(ht->table != NULL){
			free(ht->table);
		}
		free(ht);
	}
	else{
		return;
	}
}

/**************** hashtable_iterate() ****************/
/* Iterate over all items in hashtable (in undefined order):
 * call itemfunc for each item, with (arg, key, data).
 */
void hashtable_iterate(hashtable_t *ht, void (*itemfunc)(void *arg, const char *key, void *data), void *arg){

	if( ht == NULL || itemfunc == NULL){
		return;
	}
	else if (ht->table == NULL){
		printf("Error iterating as ht->table is null.\n");
	}
	else{
		for (int i =0; i < ht->slot_number; i++){
			set_iterate(ht->table[i], itemfunc , arg);
		}
	}

}

/**************** hashtable_print() ****************/
/* 
 * print out the hashtable to a file
 */

 void hashtable_counter_print(hashtable_t *ht, FILE *fp, void (*itemfunc)(FILE *fp, void *ctrs)){

 	if (ht == NULL || fp == NULL){
 		printf("Error printing table\n");
 		return;
 	}
 	else{
 		for (int i = 0; i < ht->slot_number; i++){
 			set_print(ht->table[i], fp, itemfunc);
 		}
 	}
 }






