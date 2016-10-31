/*
* settest.c - a testing file for the set.c structure
*
* Benji Hannam, July 2016
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "set.h"

int main(){

	printf("********************************\n");
	
	//Create the new set
	printf("Creating a new set.\n");
	set_t *new_set = set_new();
	if (new_set == NULL){
		printf("New set failed...Exiting\n");
		exit(1);
	}

	//Generate the keys/data
	printf("\nGenerating the keys and data for the new set.\n");
	char a[10] = "Hello";
	char c[10] = "World";
	int d = 7;

	//Insert the keys/data
	printf("\nInserting data '%s' with key '%s' into the set.\n", c, a);
	set_insert(new_set, a, c);

	printf("Inserting data '%d' with the key '%s' into the set.\n", d, c);
	set_insert(new_set, c, &d);

	printf("Inserting data '%s' with key '%s' into the set. Should already be there so will print 'Key already there'if returns false.\n", c, a);
	if(!set_insert(new_set, a, c)){
		printf("Key already there\n");
	}

	//Extract the keys/data
	printf("\nRetrieving the data with key: '%s'.\n", a);
	printf("%s\n", set_find(new_set, a));

	printf("Retrieving the data with key: '%s'.\n", c);
	printf("%d\n", *((int*)set_find(new_set, c)));
	printf("********************************\n");
}
