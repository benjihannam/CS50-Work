/*
* counterstest.c - a test file for counters.c
*
* Benji Hannam, July 2016
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "counters.h"

int main(){

	printf("********************************\n");

	//Make the new set of counters
	printf("Making a set of new counters:\n");
	counters_t *new = counters_new();
	if (new == NULL){
		printf("New set of counters failed...Exiting\n");
		exit(1);
	}

	//add in the counters
	printf("\nAdding the counters:\n");
	printf("Adding to counter 2\n");
	counters_add(new, 2);
	printf("Adding to counter 2\n");
	counters_add(new, 2);
	printf("Adding to counter 3\n");
	counters_add(new, 3);
	printf("Adding to counter 100\n");
	counters_add(new, 100);

	//retrive the counter values
	printf("\nGetting counter 2, should print '2':\n");
	printf("%d\n",counters_get(new, 2));
	printf("Getting counter 3, should print '1':\n");
	printf("%d\n",counters_get(new, 3));
	printf("Getting counter 4, should print '0':\n");
	printf("%d\n",counters_get(new, 4));
	printf("Getting counter 100, should print '1':\n");
	printf("%d\n",counters_get(new, 100));

	printf("\nDeleting the set of counters\n");
	counters_delete(new);
	printf("********************************\n");
}
