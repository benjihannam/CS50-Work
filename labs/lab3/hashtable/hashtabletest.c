/* 
 * hashtabletest.c - testfile for hastable.c
 *
 * Benji Hannam, July 2016
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hashtable.h"

 int main(){

 	printf("********************************\n");

 	//Make the new hashtable
 	printf("Making the new hashtable...\n");
 	hashtable_t *new = hashtable_new(2);
 	if (new == NULL){
		printf("New hashtable failed...Exiting\n");
		exit(1);
	}

 	//Generate the information to go in
 	printf("\nGenerating the keys and data to go in..\n");
 	char key[5] = "abc";
 	char false_key[6] = "def";
 	int a = 5;

 	printf("\nInserting the keys and data to go in..\n");
 	hashtable_insert(new, key, &a);

 	printf("\nTesting a valid key, should return '5':\n");
 	printf("%d\n", *((int*)hashtable_find(new, key)));

 	printf("Testing an invalid key, should print 'key not found':\n");
 	if(hashtable_find(new,false_key) == NULL){
 		printf("Key not found\n");
	}

	printf("********************************\n");
}
