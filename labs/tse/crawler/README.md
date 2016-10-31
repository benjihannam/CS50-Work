README for .c
Benji Hannam, July 2016

Compiling:
	make

Usage:
/* 
 * crawler.c - Crawls from a SeedURL and prints a file for each webpage visited
 * 
 * usage: To c
 * 
 *   
 * output: Writes the files to a test directory
 *
 *
 * Benji Hannam, July 2016
 */

Example command lines: ./crawler [SeedURL] [Data Directory] [Max Depth]
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/index.html ./data 0

Exit status:
  0 - success
  1 - Error with one of the arguments
  2 - Failed to allocate memory
  3 - Failed to make file

Assumptions:
- Staying within the internal playground for now

"Non"-Assumptions:
-any URL can be inputted


Limitations:
