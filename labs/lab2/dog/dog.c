/*
* dog.c     Benji Hannam    July 10, 2016
*
* A program that provides some of the funcitionality of the 'cat' command plus some extra functionality. It's called dog because dogs > cats.  It includes functions:
*     read_stdin() - handles the case where you read from stdin
*     print_help() - prints out the help information
*     executre_normal() - handles the case with a normal file
*     mapfile_check() - checks if both -m and -M have been inputted
* 
*
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h> 

//define macros
#define ERR_FILE_OPEN 2
#define MAX_FILE_LENGTH 100

//function prototypes
void read_stdin(bool m, bool M, bool n, int *count, const char mapfile[MAX_FILE_LENGTH]);
void print_help();
void execute_normal(bool m, bool M, bool n, int *count, const char filename[MAX_FILE_LENGTH], const char mapfile[MAX_FILE_LENGTH]);
bool mapfile_check(bool m, bool M);


/*
*The main function which will read a file, encrypt/decrypt it as needed and output it to stdout. 
*/
int main( int argc, char *argv[]) {

	bool n_present = false, m_present = false, M_present =false, special_present = false, help_present =false, loop = true;
	char mapfile[MAX_FILE_LENGTH], filename[MAX_FILE_LENGTH];
	int count = 1;

	//loop through the option variables and handle their cases as needed. 
	while((argc > 1) && (argv[1][0] == '-' ) && loop && (strlen(argv[1]) > 1)) {

		switch (argv[1][1]) {

			//do we have a -n?
			case 'n':
			if (strlen(argv[1]) == 2){
				n_present = true;
			}
			else{
				printf("Error: %s is not a valid option.\n", argv[1]);
				exit(1);
			}
			break;

			// do we have -m or -M?
			case 'm':
			if (strlen(argv[1]) == 2){
			m_present = true;
			}
			else{
				printf("Error: %s is not a valid option.\n", argv[1]);
				exit(1);
			}
			break;

			case 'M':
			if (strlen(argv[1]) == 2){
			M_present = true;
			}
			else{
				printf("Error: %s is not a valid option.\n", argv[1]);
				exit(1);
			}
			break;

			// do we have -h?
			case 'h':
			if (strlen(argv[1]) == 2){
				help_present = true;
				print_help();
				exit(0);
			}
			else{
				printf("Error: %s is not a valid option.\n", argv[1]);
				exit(1);
			}
			break;

			// do we have --help or a -?
			case '-':
			if ((strcmp(argv[1], "--help")) == 0){
				help_present = true;
				print_help();
				exit(0);
			}
			else if (strlen(argv[1]) == 1){
				special_present = true;
				loop = false;
			}
			else {
				printf("Error: %s is not a valid option.\n", argv[1]);
				exit(1);
			}
			break;

			//default case
			default:
				printf("Error: %s is not a valid option.\n", argv[1]);
				exit(1);
				break;

		} 
		argc--; argv++;
	}

	//check if both -m and -M have been used and exit if so
	if (mapfile_check( m_present, M_present)){
		exit(2);
	}
	else if ((m_present || M_present) && (argc > 1)){
		strcpy(mapfile, argv[1]);
		argc--; argv++;
	}
	else if (m_present || M_present){
		printf("No mapfile given.\n");
		exit(3);
	}

	//now we can loop through and handle the files, argc and argv should have incremented/decremented so that our arrays reflect only the files
	if (argc == 1){
		read_stdin(m_present, M_present, n_present, &count, mapfile);
	}

	while (argc > 1){

		// Do we have the special file?
		if (argv[1][0] == '-' && (strlen(argv[1]) == 1)){
			read_stdin(m_present, M_present, n_present, &count, mapfile);
		}
		// otherwise handle it like a normal file
		else {
			strcpy(filename, argv[1]);
			execute_normal(m_present, M_present, n_present, &count, filename, mapfile);
		}
		argc--; argv++;
	}
}

/****************************
 * The function carried out if no filenames are given
 */
void read_stdin(bool m, bool M, bool n, int *count, const char mapfile[MAX_FILE_LENGTH]){

	int current;
	char before, after;; //current char being read, the first 
	FILE *fp;
	bool change, print_count = false, new_file = true;

	//looping through each char in the stdin 
	while ((current = fgetc(stdin)) != EOF) {

		// if we have -n then print the line numbers
		// if we are opening a new file then print the count
		if ((new_file) && n){
			printf("%d. ", *count);
			*count += 1;
			new_file = false;
		}
		// if we have a new line character prep to print the line number before the next char
		else if ((current == '\n') && n){
			print_count = true;
		}
		//print the line number after a new line character
		else if (print_count){
			printf("%d. ", *count);
			*count += 1;
			print_count = false;
		}


		//if we have either -m or -M open the ciphertext file
		if (m || M){

			change = false;
			fp = fopen(mapfile, "r");

			if (fp == NULL){
				printf("Error opening the file %s, exiting... \n", mapfile);
				exit(4);
			}

			// If we have -m then encrypt the file
			else if (m){

	 			while ((fscanf(fp, "%c %c\n", &before, &after))  == 2){
	 					if (current == before){
	 						change = true;
	 						putchar(after);
	 					}
	 			}

	 			if (!change){
	 				putchar(current);
	 			}
			}

			// if we have -M then decrypt the file
			else if (M){

				while ((fscanf(fp, "%c %c\n", &before, &after))  == 2){
	 					if (current == after){
	 						change = true;
	 						putchar(before);
	 					}
	 			}
	 			if (!change){
	 				putchar(current);
	 			}
			}
			fclose(fp);
		}

		// other wise just print out the char
		else {
			putchar(current);
		}
	}
}

/****************************
 *  A function to print out the useful help information
 */
void print_help(){

	printf("dog [-n] [-m mapfile] [-M mapfile] [-] [filename]...\n");
	printf("Option [-n] numbers output lines.\n");
	printf("At most one of [-m mapfile] or [-M mapfile] may be specified.\n");
	printf(" -m will encode the inputs into ciphertext using pairs of characters read from mapfile to create a simple substitution cipher.\n");
	printf("-M will decode the inputs back into plaintext using the pairs of characters read from mapfile. The first letter is the plaintext and");
	printf("the second is the ciphertext equivalent \n");
	printf("[-] is a special filename and will result in stdin being read and written to stdout\n");
}


/****************************
 *  A function that executes the reading and writing from a normal file.
 */
void execute_normal(bool m, bool M, bool n, int *count, const char filename[MAX_FILE_LENGTH], const char mapfile[MAX_FILE_LENGTH]){

	int current;
	char before, after;; //current char being read, the first 
	FILE *fp1, *fp2;
	bool change, print_count = false, new_file = true;

	fp1 = fopen(filename, "r");

	if (fp1 == NULL){
		printf("%s is not a valid file. Exiting the program...\n", filename);
		exit(4);
	}

	//looping through each char in the file
	while ((current = fgetc(fp1)) != EOF) {

		// if we have -n then print the line numbers
		// if we are opening a new file then print the count
		if ((new_file) && n){
			printf("%d. ", *count);
			*count += 1;
			new_file = false;
		}
		// if we have a new line character prep to print the line number before the next char
		else if ((current == '\n') && n){
			print_count = true;
		}
		//print the line number after a new line character
		else if (print_count){
			printf("%d. ", *count );
			*count += 1;
			print_count = false;
		}

		//if we have either -m or -M open the ciphertext file
		if (m || M){
			change = false;
			fp2 = fopen(mapfile, "r");

			if (fp2 == NULL){
				printf("Error opening the file %s, exiting..\n", mapfile);
				exit(4);
			}

			// if we have -m then encrypt the file
			else if (m){

	 			while ((fscanf(fp2, "%c %c\n", &before, &after))  == 2){
	 					if (current == before){
	 						change = true;
	 						putchar(after);
	 					}
	 			}

	 			if (!change){
	 				putchar(current);
	 			}
			}

			// otherwise decrypt the file
			else if (M){

				while ((fscanf(fp2, "%c %c\n", &before, &after))  == 2){
	 					if (current == after){
	 						change = true;
	 						putchar(before);
	 					}
	 			}

	 			if (!change){
	 				putchar(current);
	 			}
			}
			fclose(fp2);
		}

		// if we have neither -m or -M then just print the char
		else {
			putchar(current);
		}
	}
	fclose(fp1);
}

/****************************
* A function to check if both -m and -M have been inputted
*/
bool mapfile_check(bool m, bool M){

	if (m & M){
		printf("Cannot have both -m and -M as options.\n");
		return true;
	}
	else {
		return false;
	}
}











