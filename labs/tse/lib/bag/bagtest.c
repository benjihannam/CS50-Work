/*
* bagtest.c - a testing file for bag.c
*
* Benji Hannam, July 2016
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bag.h"

void data_delete(void *data);

int main(){

	printf("********************************\n");

	//test a bag full of ints
	printf("Creating a new bag for ints.\n");
	bag_t *int_bag = bag_new(data_delete);
	if (int_bag == NULL){
		printf("New bag failed...Exiting\n");
		exit(1);
	}

	//generate the data
	printf("\nSetting up the ints to insert.\n");
	int a = 10;
	int c = 7;

	//Insert the data
	printf("\nInserting '%d' into the bag.\n", a);
	bag_insert(int_bag, &a);
	printf("Inserting '%d' into the bag.\n", c);
	bag_insert(int_bag, &c);


	//Extract the data
	printf("\nExtracing an item from the bag, will print empty if nothing in it:\n");
	int int_ret = *((int*)bag_extract(int_bag));
	if( int_ret == NULL){
		printf("Empty.\n");
	}
	else{
		printf("%d\n", int_ret);
	}

	printf("Extracing an item from the bag, will print empty if nothing in it:\n");
	int_ret = *((int*)bag_extract(int_bag));
	if( int_ret == NULL){
		printf("Empty.\n");
	}
	else{
		printf("%d\n", int_ret);
	}

	bag_delete(int_bag);
	printf("********************************\n");

	//test a bag full of strings
	printf("Creating a new bag for strings.\n");
	bag_t *string_bag = bag_new(data_delete);

	//Generate the data
	char b[7] = "Hello";

	//Insert the data
	printf("\nInserting '%s' into the bag. \n", b);
	bag_insert(string_bag, b);

	//Extract the data
	printf("\nExtracing an item from the bag, will print empty if nothing in it:\n");
	char *ret = bag_extract(string_bag);
	if(ret != NULL){
		printf("%s\n", ret);
	}
	else{
		printf("Empty.\n");
	}
	printf("Extracing an item from the bag, will print empty if nothing in it:\n");
	ret = bag_extract(string_bag);
	if(ret != NULL){
		printf("%s\n", ret);
	}
	else{
		printf("Empty.\n");
	}
	printf("********************************\n");

	bag_delete(string_bag);
}

/******************* data_delete() **************************/
 void data_delete(void *data){

	 if (data){
	 	free(data);
	 }
}
