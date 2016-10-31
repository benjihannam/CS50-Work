/*
* chill.c     Benji Hannam    July 10, 2016
*
* A program that will calculate wind chill based on an current temperature and wind velocity.  It includes functions:
*     
*     calculate_chill()
* 	  no_arguments() - returns the full table if no arguments provided
*	  one_argument() - returns all the chill factors for 5,10,15 MPH and the given temperature if only one argument provided
*	  two_arguments() - returns the chill factors for the given temperature and wind velocity 
*	  check_temp() - is the temperature within the valid range
*	  check_velocity() - is the velocity within the valid range
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// function prototypes
double calculate_chill(double temp, double velocity);
void no_arguments();
void one_argument(double temp);
void two_arguments(double temp, double velocity);
bool check_temp(double temp);
bool check_velocity(double velocity);

/* ***************************
 *  The main function
 */
int main(const int argc, const char *argv[]) {
	
	double temp, velocity;
		
		switch(argc) {

			//if no arguments print the table
			case 1:
			no_arguments();
			break;

			//if one argument then check temp and print appropriate lines if it's valid
			case 2:
				sscanf(argv[1], "%lf", &temp);
				if (check_temp(temp)){
					one_argument(temp);
				}
			break;

			//if two arguments then check temp and velocity and print appropriate lines if they are valid
			case 3:
				sscanf(argv[1], "%lf", &temp);
				sscanf(argv[2], "%lf", &velocity);
				if (check_temp(temp) & check_velocity(velocity)){
					two_arguments(temp, velocity);
				}
				break;

			//if more than two arguments
			default:
				printf("Too many arguments. \n");
				exit(1);
				break;
		}
}

/* ***************************
 *  
 * A function to calculate the wind chill for a given temperature and velocity
 */
double calculate_chill(double temp, double velocity){
	double chill = 35.74 + 0.6215*temp - 35.75*pow(velocity,0.16) + 0.4275*temp*pow(velocity,0.16);
	return chill;
}

// A function to print out the wind chill table from -10 degrees to 40 degrees in increments of 5 and velocities of 5,10 and 15 for each temperature.
void no_arguments(){

	//print out the table header
	printf("Temp     Wind     Chill\n");
	printf("-----    -----    -----\n");

	//calculate and print out the table contents
	for (float i = -10; i < 45; i = i + 5) {
		for (float j = 5; j < 20; j = j + 5) {
			
			double chill = calculate_chill(i, j);
			printf("%5.1f%9.1f%9.1f\n", i, j, chill);

		}
		printf("\n");
	}
}

/* ***************************
 *  A function to print out all the wind chill values for a given temperature with velocities of 5, 10 and 15.
 */
void one_argument(double temp) {
	
	//print out the table header
	printf("Temp     Wind     Chill\n");
	printf("-----    -----    -----\n");

	// print the table contents
	for (double j = 5; j < 20; j = j + 5){

		double chill = calculate_chill(temp, j);
		printf("%5.1f%9.1f%9.1f\n", temp, j, chill);
	}
}

/* ***************************
 *  A function to print out all the wind chill values for a given temperature with velocities of 5, 10 and 15.
 */
void two_arguments(double temp, double velocity) {

		//print out the table header
		printf("Temp     Wind     Chill\n");
		printf("-----    -----    -----\n");

		//print the line
		double chill = calculate_chill(temp, velocity);
		printf("%5.1f%9.1f%9.1f\n", temp, velocity, chill);
}

/* ***************************
 * Check if the temperature value is valid, print an error message and return false if it is not, return true if it is.
 */
bool check_temp(double temp){

	if ((temp >= 40) || (temp < -10)){
		printf("Temperature must be less than 40 and greater than -10 degrees Fahrenheit\n");
		exit(2);
		return false; 
	}
	else {
		return true;
	}
}

/* ***************************
 *  Check if the winnd velocity value is valid, print an error message and return false if it is not, return true if it is.
 */
bool check_velocity(double velocity){

	if (velocity < 0.5 || velocity > 15){
		printf("Wind velocity must be greater than or equal to 0.5 MPH and no greater than 15 MPH\n");
		exit(2);
		return false;
	}
	else {
		return true;
	}	
}










