README for chill.c
Benji Hannam, July 2016

Compiling:
	gcc -o chill chill.c -lm

Usage:
/* 
 * chill.c - Calculates wind chill factor
 * 
 * usage: ./chill [temperature] [wind velocity]
 * - no arguments provided will print out a table for wind chill for a temperature range
 *   of -10 to +40 degrees 
 *   Fahrenheit and a wind speed of 5,10 and 15 MPH.
 * - one argument give will result in the table with the wind chill at 5,10 and 15 MPH for the
 *   temperature of
 *   the given argument.
 * - two arguments will return the wind chill for the temperature of the given first
 *   argument (within the range
 *   of -10 to +40 no inclusive) and wind speed of the given second argument (within the
 *   range of 0.5 to 15 MPH inclusive).
 *
 * output: prints the result to stdout.
 *
 * Benji Hannam, July 2016
 */

Example command lines:
	
	./dog
	./dog 30
	./dog 30.5 12

Exit status:
  0 - success
  1 - too many arguments
  2 - Arguments are not in range

Assumptions:
- the inputted arguments are numbers and not strings
- the first argument will be the inputted temperature
- the second argument will be the inputted

Non-Assumptions:
-any positive or negative number can be inputted

Limitations:
- only calculates for the ranges given