/*
* set.c - a basic set structure
*
* Benji Hannam, July 2016
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "set.h"

/********************global variables ***********/

/*************** local types ********************/
typedef struct item  
{
   char *key;
   void *data;
   struct item *next;
}item_t;
/*************** global types *******************/
typedef struct set {
	item_t *head;

}set_t;

/**************** global functions ****************/

/**************** set_new() ****************/
//create a new set
set_t *set_new(void){
	set_t *new_set = malloc(sizeof(set_t));
	if (new_set == NULL){
		printf("Error allocating memory to new_set\n");
		free(new_set);
		return NULL;
	}
	new_set->head= malloc(sizeof(item_t));
	if (new_set->head == NULL){
		printf("Error allocating memory to new_set->head\n");
		free(new_set->head);
		free(new_set);
		return NULL;
	}
	new_set->head->next= NULL;
	new_set->head->key = "";
	new_set->head->data = NULL;
	return new_set;
}

/**************** set_insert() ****************/
//inserts a new string/data pair into the set, returns false if the key is already there and true if the pair is added.
bool set_insert(set_t *set, char *key, void *data){
	// if the set is null then return false
	if (set == NULL){
		return false;
	}
	//assign the current item to be the head
	item_t *current = set->head;
	//go through the list until you reach the last item or you find a item with the same key
	while((current->next != NULL) && (strcmp(current->key, key) != 0)){
		current = current->next;
	}
	// if we found the key
	if (strcmp(current->key, key) == 0){
		free(current);
		return false;
	}
	//otherwise generate a new item and put it in
	else{
		item_t *new_item = malloc(sizeof(item_t));
		if(new_item == NULL){
			printf("Error allocating memory to the new item.\n");
			free(new_item);
			return false;
		}
		new_item->key = malloc(strlen(key)+1);
		if (new_item->key == NULL) {
      		printf("Error allocating memory for the key.\n");
      		// cleanup and return error
      		free(new_item->key);
      		free(new_item);
      		return false;
      	}
		strcpy(new_item->key, key);
		new_item->data = data;
		new_item->next = NULL;
		current->next = new_item;
		return true;
	}
}

/**************** set_find() ****************/
//Looks for the pair with key, returns the data if its there, returns NULL if it isn't
void *set_find(set_t *set, char *key){
	//assign the current item to be the head
	item_t *current = set->head;
	//go through the list until you reach the last item or you find a item with the same key
	while((current->next != NULL) && (strcmp(current->key, key) != 0)){
		current = current->next;
	}
	// if we found the key
	if (strcmp(current->key, key) == 0){
		return current->data;
	}
	//otherwise return NULL
	else{
		return NULL;
	}
}

