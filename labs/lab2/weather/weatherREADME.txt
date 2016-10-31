README for weather.c
Benji Hannam, July 2016

Compiling:
	gcc -o weather weather.c mycurl.c -lcurl

Usage:
/* 
 * weather.c - gets some weather information from NOAA’s weather.gov for a specified 4 
 *             digit code.
 *
 * usage: ./weather [CODE]
 *      
 * 	Will return the given information for the location given by the code in the form:
 *
 *      Credit: NOAA's National Weather Service
 *	Location: Mount Washington, NH
 *	station_id: KMWN
 *	Observation_time: Last Updated on Jul 14 2016, 1:47 pm EDT
 *	Weather: Fog and Windy
 *	Temperature_string: 59.0 F (15.0 C)
 *	Relative_humidity: 100
 *	Wind_string: from the Southwest at 27.6 gusting to 38.0 MPH (24 gusting to 33 KT)
 *	Visibility: 0.00
 *  
 * output: Writes to the information to stdout.
 *
 *
 * Benji Hannam, July 2016
 */

Example command lines:


Exit status:
  0 - success
  1 - wrong number of arguments
  2 - Error with location code
  3 - location code not found
  4 - curl_easy_perform failed 

Assumptions:
- Assumes that weather.gov is active

"Non"-Assumptions:
-any length digit code with any ASCII characters can be inputted
-

Limitations:
- Only can get information for one code at a time