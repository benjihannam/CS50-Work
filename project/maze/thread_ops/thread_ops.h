/*
* thread_ops.h header file for the threads module
* Threads module wraps all functionality of the avatar threads 
*
*
* Benjamin Littlejohn and Larissa Chen, August 2016
* Team: core_dumped_in_a_maze
*/
#include "../lib/amazing.h"
#include "../lib/mazestruct.h"
#include "../lib/avatar.h"

#ifndef __THREAD_OPS_H
#define __THREAD_OPS_H

/**************** global structs ******************/
//struct to store data needed to run avatar thread
typedef struct thread_data { 
    int id;
    int maze_port;
    char *host_name;
    int *return_status;
} thread_data_t;

/**************** avatar_thread ****************/
/*
 * Managerial function that calls main function that runs each thread. 
 *
 * Maps params struct to individual parameters to pass into the "run_avatar_thread"
 * 	which runs avatar threads by communicating with the server and making moves 
 * 	when it is the avatar's turn to go
 *
 * Returns error or success code from "run_avatar_thread".
 */
void *avatar_thread(void *params);

#endif
