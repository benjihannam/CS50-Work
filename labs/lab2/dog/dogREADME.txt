README for dog.c
Benji Hannam, July 2016

Compiling:
	gcc -o dog dog.c

Usage:
/* 
 * dog.c - provides some of the functionality of the Unix ‘cat’ command plus the ability
 * to encrypt and decrypt stdin or a inputted file and print the result to stdout.
 *
 * usage: ./dog [—help] or ./dog [-h] prints useful helpful information about the usage
 *        of the program.
	  ./dog [-n] [-m mapfile] [-M mapfile] [-] [filenames…]  
 *
 *  - no arguments: ./dog will read from stdin and write to stdout
 *  - one or more arguments: ./dog will read from each of the files and write the result
 *    to stdout.
 *  - if the special file character [-] is specified at any point in the list of files
 *    then the program will read from stdin and write to stdout until (Ctrl + D) is 
 *    inputed.
 *   
 * output: The program will output to stdout.
 *
 *
 * Benji Hannam, July 2016
 */

Example command lines:

	./dog
	./dog -m mapfile.txt filename
	./dog -n -M mapfile.txt filename -

Exit status:
  0 - success
  1 - invalid option give
  2 - both -m and -M options
  3 - No mapfile provided
  4 - Invalid file provided

Assumptions:

- The options will be given before any filenames and in the order stated above.
- when given the options they will be given one at a time (e.g -n -m mapfile) not
  together (e.g -nm mapfile)
- the special filename ‘-‘may be specified at any time in the list of filenames
- the mapfile will only handle the 26 lowercase letters of the alphabet and the 26 upper
  uppercase letters of the alphabet. Each line will be of the form: 
  “plaintext_character ciphertext_character”
  E.g the line “a b” will turn all ‘a’ characters into ‘b’ characters under the option -m
  and all ‘b’ characters to ‘a’ characters under the -M option.

Non-Assumptions:
-The name of the file can contain any characters as long as it is a valid text file

Limitations: