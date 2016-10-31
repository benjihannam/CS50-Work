/*
* bag.c - a simple bag structure
*
* Delete function done using example code from CS50.
* Benji Hannam, July 2016
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bag.h"

/********************global variables ***********/

/*************** local types ********************/
typedef struct item  
{
   void *data;
   struct item *next;
}item_t;

/*************** global types *******************/
typedef struct bag {
	item_t *head;
	void (*itemdelete)(void *data);

} bag_t;

/**************** global functions ****************/

/**************** bag_new() ****************/
//creates a new bag
bag_t *bag_new(void (*itemdelete)(void *data)){
	bag_t *new_bag = malloc(sizeof(bag_t));
	if (new_bag == NULL){
		printf("Error allocating memory to new_bag\n");
		return NULL;
	}
	new_bag->head= malloc(sizeof(item_t));
	if (new_bag->head == NULL){
		printf("Error allocating memory to new_bag->head\n");
		return NULL;
	}
	new_bag->head->next= NULL;
	new_bag->itemdelete = itemdelete;
	return new_bag;
}

/**************** bag_insert() ****************/
// insert an item into a bag
void bag_insert(bag_t *bag, void *data){
	//assign the current item to be the head of the b
	item_t *current = bag->head;
	//go through the list until you reach the last item
	while(current->next != NULL){
		current = current->next;
	}
	//when at the end, create a new item and add it
	item_t *new_item = malloc(sizeof(item_t));
	if(new_item == NULL){
		printf("Error allocating memory to the new item.\n");
		return;
	}
	new_item->data = data;
	new_item->next = NULL;
	current->next = new_item;
}

/**************** bag_extract() ****************/
//remove an item from the bag and return it to the called
void *bag_extract(bag_t *bag){
	item_t *temp;
	void *extracted_item;
	if((bag->head->next) == NULL){
		return NULL;
	}
	else{
		extracted_item = bag->head->next->data;
		temp = bag->head->next;
		bag->head->next = temp->next;
		free(temp);
		return extracted_item;
	}
}

/**************** bag_delete() ****************/
void bag_delete(bag_t *bag){
	if (bag != NULL){
		//go through the list until you reach the last item
		while(bag->head->next != NULL){
			if (bag->itemdelete != NULL){
				(*bag->itemdelete)(bag->head->next->data);
			}
			item_t *temp = bag->head->next->next;
			free(bag->head->next);
			bag->head->next = temp;
		}
		free(bag->head);
		free(bag);
	}
}

/**************** bag_iterate() ****************/
/* Iterate over all items in bag (in undefined order):
 * call itemfunc for each item, passing (arg, data).
 */
void bag_iterate(bag_t *bag, void (*itemfunc)(void *arg, void *data), void *arg){

	item_t *current = bag->head;

	if (bag == NULL || itemfunc == NULL){
		return;
	}

	while(current->next != NULL){
		(*itemfunc)(arg, current->next->data);
		current= current->next;
	}

}



