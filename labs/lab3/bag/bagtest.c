/*
* bagtest.c - a testing file for bag.c
*
* Benji Hannam, July 2016
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bag.h"


int main(){

	int *int_ret;

	printf("********************************\n");

	//test a bag full of ints
	printf("Creating a new bag for ints.\n");
	bag_t *int_bag = bag_new();
	if (int_bag == NULL){
		printf("New bag failed...Exiting\n");
		exit(1);
	}

	//generate the data
	printf("\nSetting up the ints to insert.\n");
	int a = 10;
	int c = 7;
	int f = 1000;
	int g = 1232342;

	//Insert the data
	printf("\nInserting '%d' into the bag.\n", a);
	bag_insert(int_bag, &a);
	printf("Inserting '%d' into the bag.\n", c);
	bag_insert(int_bag, &c);
	printf("Inserting '%d' into the bag.\n", f);
	bag_insert(int_bag, &f);
	printf("Inserting '%d' into the bag.\n", g);
	bag_insert(int_bag, &g);

	while ((int_ret = bag_extract(int_bag)) != NULL){
		printf("\nExtracing an item from the bag, will print empty if nothing in it:\n");
		printf("%d\n", *int_ret);
	}
	printf("\nExtracing an item from the bag, will print empty if nothing in it:\n");
	printf("Empty.\n");

	//Extract the data
	// printf("\nExtracing an item from the bag, will print empty if nothing in it:\n");
	// int_ret = *((int*)bag_extract(int_bag));
	// if( int_ret == NULL){
	// 	printf("Empty.\n");
	// }
	// else{
	// 	printf("%d\n", int_ret);
	// }

	// printf("Extracing an item from the bag, will print empty if nothing in it:\n");
	// int_ret = *((int*)bag_extract(int_bag));
	// if( int_ret == NULL){
	// 	printf("Empty.\n");
	// }
	// else{
	// 	printf("%d\n", int_ret);
	// }

	printf("********************************\n");

	//test a bag full of strings
	printf("Creating a new bag for strings.\n");
	bag_t *string_bag = bag_new();
	char *ret;
	//Generate the data
	char b[7] = "Hello";
	char k[20] = "Please work";
	char l[25] = "World";

	//Insert the data
	printf("\nInserting '%s' into the bag. \n", b);
	bag_insert(string_bag, b);
	printf("\nInserting '%s' into the bag. \n", k);
	bag_insert(string_bag, k);
	printf("\nInserting '%s' into the bag. \n", l);
	bag_insert(string_bag, l);

	//Extract the data
	while ((ret = bag_extract(string_bag)) != NULL){
		printf("\nExtracing an item from the bag, will print empty if nothing in it:\n");
		printf("%s\n", ret);
	}
	printf("\nExtracing an item from the bag, will print empty if nothing in it:\n");
	printf("Empty.\n");

	//printf("\nExtracing an item from the bag, will print empty if nothing in it:\n");
	// char *ret = bag_extract(string_bag);
	// if(ret != NULL){
	// 	printf("%s\n", ret);
	// }
	// else{
	// 	printf("Empty.\n");
	// }
	// printf("Extracing an item from the bag, will print empty if nothing in it:\n");
	// ret = bag_extract(string_bag);
	// if(ret != NULL){
	// 	printf("%s\n", ret);
	// }
	// else{
	// 	printf("Empty.\n");
	// }
	printf("********************************\n");
}
