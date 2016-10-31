/*
* counters.c - a structure to keep a set of counters
*
* Benji Hannam, July 2016
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "counters.h"

/********************global variables ***********/

/*************** local types ********************/
typedef struct counter  
{
   int key;
   int count;
   struct counter *next;
}counter_t;

/*************** global types *******************/
typedef struct counters {
	counter_t *head;

}counters_t;

/**************** global functions ****************/

/**************** counters_new() ****************/
//creates a new set of counters
counters_t *counters_new(void){
	counters_t *new_counters = malloc(sizeof(counters_t));
	if (new_counters == NULL){
			printf("Error allocating memory to new_counters\n");
			free(new_counters);
			return NULL;
		}
	new_counters->head = malloc(sizeof(counter_t));
	if (new_counters->head == NULL){
			printf("Error allocating memory to new_counters\n");
			free(new_counters->head);
			return NULL;
		}
	new_counters->head->next = NULL;
	new_counters->head->key = -1;
	return new_counters;
}

/**************** counters_add() ****************/
// add a new counter if the key doesn't exist, if it does then increment the count of that counter
void counters_add(counters_t *ctrs, int key){
	//assign the current counter to be the head
	counter_t *current = ctrs->head;
	//go through the list until you reach the last item or you find a counter with the same key
	while((current->next != NULL) && (current->key != key)){
		current = current->next;
	}
	// if we found the key increment the count
	if (current->key == key){
		current->count += 1;
	}
	// otherwise add a new counter with count = 1 and the provided key.
	else if (current->next == NULL){
		counter_t *new_counter = malloc(sizeof(counter_t));
		if (new_counter == NULL){
			printf("Error allocating memory to the new_counter\n");
			exit(0);
		}
		current->next = new_counter;
		new_counter->key = key;
		new_counter->count = 1;
		new_counter->next = NULL;
	}
}

/**************** counters_get() ****************/
//get the current value for a key, returns 0 if the key is not found
int counters_get(counters_t *ctrs, int key){
	//assign the current counter to be the head
	if(ctrs == NULL){
		return -1;
	}
	counter_t *current = ctrs->head;
	//go through the list until you reach the last item 
	while((current->next != NULL) && (current->key != key)){
		current = current->next;
	}
	// if we found the key return the count
	if (current->key == key){
		return current->count;
	}
	// otherwise return 0
	else{
		return 0;
	}
}

/**************** counters_delete() ****************/
//delete the counters
void counters_delete(counters_t *ctrs){
	counter_t *head = ctrs->head;
	counter_t *temp = NULL;

	// while we aren't at the end of the linked list of counters
	while(head->next != NULL){
		//set the termporary pointer to the first counter
		temp = head->next;
		//free the first counter
		head->next = head->next->next;
		free(temp);
	}
	//free the head and the counters
	free(head->next);
	free(head);
	free(ctrs);
}

// int main(){
// 	counters_t *new = counters_new();
// 	counters_add(new, 2);
// 	counters_add(new, 2);
// 	counters_add(new, 3);
// 	counters_add(new, 100);
// 	printf("%d\n",counters_get(new, 2));
// 	printf("%d\n",counters_get(new, 3));
// 	printf("%d\n",counters_get(new, 4));
// 	printf("%d\n",counters_get(new, 100));
// 	counters_delete(new);
// }
