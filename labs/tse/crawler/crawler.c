/*
 * crawler.c - a program to crawl through a webpage and all the URLs on that web page to a certain depth.
 *
 * Benji Hannam, July 2016
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <unistd.h>
#include <curl/curl.h> 
#include "../common/web.h"
#include "../lib/bag/bag.h"
#include "../lib/hashtable/hashtable.h"
#include <unistd.h>
#include <ctype.h>

/*******************************************************************************************************/

//define macros
#define SLEEP_INTERVAL 1

/*******************************************************************************************************/

//function prototypes
bool IsWritableDirectory(char *dir);
bool create_webpage(char *url, int depth, WebPage *page);
void create_pagename(char *directory, int page_number, char* filename);
void write_file(char *filename, char *url, char* html, int depth);
void web_delete(WebPage *page);
void data_delete(void *data);
void new_hashtable_delete(hashtable_t *ht);

/*******************************************************************************************************/
// The main function

 int main(int argc, char *argv[]){

 
 	//check that the arguments are all valid:

 	//check that we have 3 arguments
 	if (argc != 4){
 		printf("Incorrect number of arguments, expecting 3, only had %d\n", argc - 1);
 		exit(1);
 	}

 	//variables to hold the arguments
 	char seedURL[strlen(argv[1] + 1)];
 	char page_dir[strlen(argv[2] + 1)];
 	int max_depth;
 	strcpy(page_dir, argv[2]);
 	strcpy(seedURL, argv[1]);

 	// check if the directory is valid
 	if (!IsWritableDirectory(page_dir)){
 		printf("The directory is not valid.\n");
 		exit(1);
 	}

 	//now check that the seed URL is internal
 	if (!IsInternalURL(seedURL)){
 	 	printf("%s is not an internal url and is not valid.\n", seedURL );
 	 	exit(1);
 	}

 	if (!NormalizeURL(seedURL)){
 		printf("The seed URL %s cannot be normalized...\n", seedURL);
 		exit(1);
 	}

 	//Now check the depth argument
 	for (int i = 0; i <strlen(argv[3]); i++){
 		if(isdigit(argv[3][1]) != 0){
 			printf("Max depth provided is not a valid number.\n");
 			exit(1);
 		}
 	}
 	sscanf(argv[3], "%d", &max_depth);

 	//set up the bag
 	bag_t *URL_bag = bag_new(data_delete);
 	if(URL_bag == NULL){
 		printf("New bag failed...Exiting\n");
		exit(2);
 	}

 	//More variable to be used now that we have checked the arguments
 	WebPage *new_webpage, *seed_page, *current;
 	int page_number = 1;
 	int pos;
 	char* result, *filename = NULL;
 	
 	//Create the hashtables
 	hashtable_t *URL_hashtable = hashtable_new(2000, data_delete);
 	if(URL_hashtable == NULL){
 		printf("New hashtable failed.\n");
 		exit(2);
 	}

 	//Generate the seed page
 	seed_page = malloc(sizeof(WebPage));
    if (seed_page == NULL){
    	printf("Failed to allocated memory to webpage structure.\n");
 		free(seed_page);
 		exit(2);
   	}
   	if(!create_webpage(seedURL, 0, seed_page)){
   		exit(3);
   	}

 	//now put it into the bag and hashtable
 	bag_insert(URL_bag, seed_page);
 	hashtable_insert(URL_hashtable, seedURL, NULL);

 	//While we still have pages in the bage
 	while ((current = bag_extract(URL_bag)) != NULL){
 		sleep(SLEEP_INTERVAL);

 		//Write the file for the page and put it in the directory
 		filename = malloc(strlen(page_dir) + 50);
 		if (filename == NULL){
 			printf("Error allocating memory to filename\n");
 			exit(2);
 		}
 		create_pagename(page_dir, page_number, filename);
 		write_file(filename, current->url, current->html, current->depth);
 		page_number += 1;

 		//if the page is less then the max depth
	 	if(current->depth < max_depth){
		 	// Now crawl the page 
	 		pos = 0;
			while ((pos = GetNextURL(current->html, pos, current->url, &result)) > 0) {


            	//Normalise and check
            	if (NormalizeURL(result)){

            		//if its internal
            		if (IsInternalURL(result)){

            			//check if its already in the hashtable
            			char *str;
            			str = malloc(1000*sizeof(char));
            			if(hashtable_insert(URL_hashtable, result, str)){

            				// // If it isnt, create a webpage for the new URLS
					    	new_webpage = malloc(sizeof(WebPage));
						    if (new_webpage == NULL){
						    	printf("Failed to allocated memory to webpage structure.\n");
						    	//free(url);
						    	web_delete(new_webpage);
						 		exit(2);
						   	}
					 		// fetch the page
					 		if (create_webpage(result, (current->depth +1), new_webpage)){
						 		//put it in the bag
						 		bag_insert(URL_bag, new_webpage);
						 	}
					 	}
					}
				 }
				if (result != NULL){
					free(result);
				}
		 	}
		}
		//delete the webpage
		if (current != NULL){
			web_delete(current);
		}
		if (filename != NULL){
			free(filename);
		}
	}
	printf("Bag empty\n");

	//clean up and delete hashtable/bag
	if (URL_bag != NULL){
		bag_delete(URL_bag);
	}
	if (URL_hashtable != NULL){
		hashtable_delete(URL_hashtable);
	}
}

 /*******************************************************************************************************/

 /******************* IsWritableDirectory() **************************/
 /* 
 * A function to check if a directory is valid
 */
 bool IsWritableDirectory(char *dir){

 	//create a filename to test opening
 	char append[15] = "/test";
 	char filename[(strlen(dir) + strlen(append) + 1)];
 	strcpy(filename, dir);
 	strcat(filename, append);

 	//now open the file
 	FILE *fp;
 	fp = fopen(filename, "w");

 	if (fp == NULL){
 		return false;
 	}
 	//close the file
 	else{
 		fclose(fp);
 		return true;
 	}

 }

/******************* create_webpage() **************************/
 /* 
 * A function to create a webpage for a passed in url
 * returns true if it suceeds and false if it fails
 */
 bool create_webpage(char *url, int depth, WebPage *page){

 	// assign all the variables
 	page->url = malloc(strlen(url) +1);
 	if (page->url == NULL){
 		printf("Error allocating memory to page->url\n");
 		free(page->url);
 		free(page);
 		exit(2);
 	}
	strcpy(page->url, url);
	page->html = NULL;
	page->depth = depth;

	//get the html
	if(GetWebPage(page)) {
		return true;
	}
	else{
		printf("Error fetching page %s\n", page->url);
		free(page->html);
		free(page->url);
		free(page);
		return false;
	}

 }

 /******************* create_pagename() **************************/
 /* 
 * A function to create the name of the file that the webpage will be written to
 */
 void create_pagename(char *directory, int page_number, char* filename){
 	// generate the pagenumber as a string
 	char value[6];
 	snprintf(value, 6, "%d", page_number);

 	//generate the directory part of the string
 	strcpy(filename, directory);

 	//put them together
 	strcat(filename, "/");
 	strcat(filename,value);

 }

 /******************* write_file() **************************/
 /* 
 * A function to write the relevant information from the webpage to the file
 */
 void write_file(char *filename, char *url, char* html, int depth){

 	FILE *fp;
 	char url_copy[strlen(url) +1];
 	char html_copy[strlen(html) +1];
 	strcpy(url_copy, url);
 	strcpy(html_copy, html);

 	//open the file
 	fp = fopen(filename, "w");
 	if(fp == NULL){
 		printf("Failed to create the text file.\n");
 		exit(3);
 	}

 	//print the information to the file
 	fputs(url_copy, fp);
 	fputs("\n", fp);
 	fputs(html_copy, fp);
 	fprintf(fp, "%d\n", depth);


 	//close
 	fflush(fp);
 	fclose(fp);
 	printf("Made page for %s\n", url);

 }

/******************* web_delete() **************************/
 /* 
 * A function to delete a webpage structure
 */
 void web_delete(WebPage *page){
 	if (page->url != NULL){
 		free(page->url);
 	}
 	if (page->html != NULL){
 		free(page->html);
 	}
 	if(page != NULL){
 		free(page);
 	}
 }

 /******************* data_delete() **************************/
 /* 
 * A function to delete some data
 */
 void data_delete(void *data){

	 if (data != NULL){
	 	free(data);
	 }
}
/*******************************************************************************************************/

