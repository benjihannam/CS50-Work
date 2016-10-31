/*
* weather.c     Benji Hannam    July 12, 2016
*
* A program that requests a weather report from weather.gov for a specific location and prints part of the response. It includes functions:
*     
*     print_info() - prints the information in the correct format.
*
*/

#include "mycurl.h"
#include <curl/curl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


// Define macros
#define INFO_LENGTH 25

//function prototypes
void print_info(char info_open[INFO_LENGTH], char info_close[INFO_LENGTH], struct curlResponse *s);


int main(int argc, char *argv[])
{
    /* local variables */
    char code[5], url[100], ch;
    bool code_found;

    /* check args */
    if (argc == 1){
    	printf("./weather needs one arg, a location code like KMWN.  To look up location codes, see http://w1.weather.gov/xml/current_obs/seek.php?state=.\n");
    	exit(1);
    }
    else if (argc > 2){
    	printf("Error: Too many arguments. One argument expected.\n");
    	exit(1);
    }
    else if (strlen(argv[1]) != 4){
    		printf("Error: Location code must be 4 characters.\n");
    		exit(2);
    }
    else{
    	for (int i=0; i < 4; i++){
    		ch = argv[1][i];
    		if (!(ch >= 'A' && ch <= 'Z')){
    			printf("Error: Location code contains a lowercase character.\n");
    			exit(2);
    		}
    	}
    }

    strcpy(code, argv[1]);

    /* curl stuff */
    CURL *curl;
    CURLcode res;
    struct curlResponse s;

    /* Format string for the URL we will request */
    char *APIurl  = "http://w1.weather.gov/xml/current_obs/%s.xml";

    /* allocate and initialize the output area */
    init_curlResponse(&s);

    /* create the URL from the format string */
    sprintf(url, "http://w1.weather.gov/xml/current_obs/%s.xml", code);
    /* ... */

    /* initialize curl */
    curl = curl_easy_init();

    if(curl) {
        /* set the URL we want to visit */
        curl_easy_setopt(curl, CURLOPT_URL, url );
        
        /* weather.gov expects USERAGENT option */
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "cs50");
                
        /* set the function curl should call with the result */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        /* set the buffer into which curl should place the data */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        
        /* make sure it worked */
        if (res != 0){
        	printf("%s\n", curl_easy_strerror(res));
        	exit(4);
        }


        /* check: if the weather station code wasn't found, */
        /* we get back HTML instead of XML  */
        char check[6]; 
        strncpy(check, s.ptr, 5);
        if (strcmp(check, "<?xml") == 0){
        	code_found = true;
        }
        else {
        	code_found = false;
        	printf("Error: the weather station code %s was not found.\n", code);
        	exit(3);
        }

        /* otherwise, select the desired output from the results */

        //the information parts we want
        char credit_open[INFO_LENGTH] = "<credit>", credit_close[INFO_LENGTH] = "</credit>";
        char location_open[INFO_LENGTH] = "<location>", location_close[INFO_LENGTH] = "</location>";
        char station_id_open[INFO_LENGTH] = "<station_id>", station_id_close[INFO_LENGTH] = "</station_id>";
        char observation_time_open[INFO_LENGTH] = "<observation_time>", observation_time_close[INFO_LENGTH] = "</observation_time>";
        char weather_open[INFO_LENGTH] = "<weather>", weather_close[INFO_LENGTH] = "</weather>";
        char temperature_string_open[INFO_LENGTH] = "<temperature_string>", temperature_string_close[INFO_LENGTH] = "</temperature_string>";
        char relative_humidity_open[INFO_LENGTH] = "<relative_humidity>", relative_humidity_close[INFO_LENGTH] = "</relative_humidity>";
        char wind_string_open[INFO_LENGTH] = "<wind_string>", wind_string_close[INFO_LENGTH] = "</wind_string>";
        char visibility_mi_open[INFO_LENGTH] = "<visibility_mi>", visibility_mi_close[INFO_LENGTH] = "</visibility_mi>";

        // print the information
        printf("Credit: ");
        print_info(credit_open, credit_close, &s);

        printf("Location: ");
        print_info(location_open, location_close, &s);

        printf("station_id: ");
        print_info(station_id_open, station_id_close, &s);

        printf("Observation_time: ");
        print_info(observation_time_open, observation_time_close, &s);

        printf("Weather: ");
        print_info(weather_open, weather_close, &s);

        printf("Temperature_string: ");
        print_info(temperature_string_open, temperature_string_close, &s);

        printf("Relative_humidity: ");
        print_info(relative_humidity_open, relative_humidity_close, &s);

        printf("Wind_string: ");
        print_info(wind_string_open, wind_string_close, &s);

        printf("Visibility: ");
        print_info(visibility_mi_open, visibility_mi_close, &s);

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
}

/****************************
 *  A function to print out the useful help information
 */
 void print_info(char info_open[INFO_LENGTH], char info_close[INFO_LENGTH], struct curlResponse *s){

 	//variables
 	char *close, *open;

 	// Get the pointer locations for the information 
 	open = strstr((*s).ptr, info_open);
    close = strstr((*s).ptr, info_close);

    // loop through the information characters and print them out
    for (int i = 0; i <= (close - open - strlen(info_close)); i++){
       	int j = i + (strlen(info_open));
        printf("%c", open[j]);
    }
    printf("\n");
 }











