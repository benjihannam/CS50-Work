/*
 * thread_ops.c
 * Authors: Benjamin Littlejohn and Larissa Chen
 * Team: core_dumped_in_a_maze
 * Date: August 2016
 * Purpose: Runs the avatar threads for cs50 X16 project Maze Challenge
 */

/***************** Return Statuses ******************/
#define SUCCESS (0)
#define FAILED (1)
#define UNCLEAR (2)
#define MALLOC_ERROR (3)
#define SOCKET_NOT_CREATED (4)
#define UNKNOWN_HOST (5)
#define FAILED_CONNECTION (6)
#define MESSAGE_FAILED_TO_BE_WRITTEN (7)
#define AVATAR_NOT_CREATED (8)

/********************* includes *********************/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "thread_ops.h"
#include "../lib/mazestruct.h"

/************************* local function prototypes **********************/
static int run_avatar_thread(int avatar_id, mazestruct_t *maze, int maze_port,
	char *hostname);
static Avatar *initialize_avatar(int id);
static int connect_to_server(int *comm_sock, char *hostname, int maze_port);
static int send_am_avatar_ready(Avatar *my_avatar, AM_Message *msg_buff,
	int comm_sock);
void wait_for_response(int comm_sock, AM_Message *msg_buff);
static int solve_maze(Avatar *my_avatar, mazestruct_t *maze, int comm_sock,
 	AM_Message *msg_buff);

/********************* external global declarations *********************/
extern mazestruct_t *maze;
extern pthread_mutex_t my_turn;

/*************************** avatar_thread ******************************/
/*
 * Method passed to thread_create() to run and exit an avatar thread
 */
void *avatar_thread(void *params) {
  thread_data_t *thread_data = (thread_data_t *)params;

  *(thread_data->return_status) = run_avatar_thread(thread_data->id, maze,
	  thread_data->maze_port, thread_data->host_name);

  pthread_exit(NULL);
}

/************************** run_avatar_thread **************************/
/*
 * Runs avatar threads by communicating with the server and making moves 
 * 	when it is the avatar's turn to go
 */
static int run_avatar_thread(int avatar_id, mazestruct_t *maze, int maze_port,
	char *hostname) {

    //declarations
    int ret_status = SUCCESS;
    int comm_sock;

    //initialize needed structs
    AM_Message *msg_buff = malloc(sizeof(AM_Message));
    Avatar *my_avatar = initialize_avatar(avatar_id);

    if (msg_buff == NULL || my_avatar == NULL) {
	     fprintf(stderr, "Due to malloc error, avatar %d couldn't contact"
		     " server.\n", avatar_id);
	      ret_status = MALLOC_ERROR;
    }
    else if ((ret_status = connect_to_server(&comm_sock, hostname, maze_port)
		!= SUCCESS)) {} //jump to clean up and exit
    else if ((ret_status = send_am_avatar_ready(my_avatar, msg_buff, comm_sock)
		!= SUCCESS)) {
	     close(comm_sock); //then jump to clean up and exit
    }
    else {
      ret_status = solve_maze(my_avatar, maze, comm_sock, msg_buff);
    }

    // clean up
    close(comm_sock);
    free(my_avatar);
    free(msg_buff);

    return ret_status;
}

/*
 * Creates a new avatar struct with the given id
 * Returns NULL upon failure or a pointer to the Avatar when successful
 * Caller is responsible for freeing the avatar
 */
static Avatar *initialize_avatar(int id) {
    Avatar *avatar = malloc(sizeof(Avatar));
    if (avatar == NULL) {
	return NULL;
    }
    else {
	     avatar->fd = id;
	     avatar->leader = id;
	      return avatar;
    }
 }

/*
 * Trys to connect to the server specified by hostname at port maze_port
 * 	using socket at address of comm_sock
 *
 * If successful then the given socket is properly connected to the given
 * 	server
 *
 * Returns 0 if the operation was successful and positive integer error codes
 * 	if not as well as prints error messages
 *
 * See return statuses in head-of-file comments for details
 */
static int connect_to_server(int *comm_sock, char *hostname, int maze_port) {

    //initialize necessary args
    *comm_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct hostent *hostp = gethostbyname(hostname);
    struct sockaddr_in server;

    //make sure initializations worked
    if (*comm_sock < 0) {
	     perror("Error creating communication socket.\n");
	      return SOCKET_NOT_CREATED;
    }
    else if (hostp == NULL) {
	     fprintf(stderr, "%s: unknown host\n", hostname);
	     return UNKNOWN_HOST;
    }

    //initialize server
    server.sin_family = AF_INET;
    server.sin_port = htons(maze_port);
    memcpy(&server.sin_addr, hostp->h_addr_list[0], hostp->h_length);

    //try to connect to server
    if (connect(*comm_sock,(struct sockaddr*)&server, sizeof(server)) < 0) {
    	perror("Connection to server failed.\n");
    	return FAILED_CONNECTION;
    }
    return SUCCESS;
}

/*
 * Sends the AM_AVATAR_READY message using the given comm_sock and msg_buff
 */
static int send_am_avatar_ready(Avatar *my_avatar, AM_Message *msg_buff,
	int comm_sock) {

    //form AM_AVATAR_READY message to send to server
    msg_buff->type = htonl(AM_AVATAR_READY);
    msg_buff->avatar_ready.AvatarId = htonl(my_avatar->fd);

    //send message to server
    if (write(comm_sock, msg_buff, sizeof(AM_Message)) < 0) {
	     fprintf(stderr, "Error writing avatar %d's move to server.\n",
		     my_avatar->fd);
         return MESSAGE_FAILED_TO_BE_WRITTEN;
    }
    printf("Avatar %d sent AM_AVATAR_READY.\n", my_avatar->fd);
    return SUCCESS;
}

/*
 * Method run by threads once everything is set up to solve the maze
 *
 * Description:
 * 	1. Initializes the avatars in the maze
 * 	2. Waits for the avatar's turn to move and makes a move according
 * 	   to heurisics described in "avatar.h" 
 * 	3. Determines the exit status of the thread based on the final 
 * 	   meessage returned by the server
 */
static int solve_maze(Avatar *my_avatar, mazestruct_t *maze, int comm_sock,
	AM_Message *msg_buff) {

    int ret_status;

    //wait for initial response from server
    wait_for_response(comm_sock, msg_buff);
    if(ntohl(msg_buff->type) == AM_AVATAR_TURN) {
	//update avatar position based on response
	my_avatar->pos.x = ntohl(msg_buff->avatar_turn.Pos[my_avatar->fd].x);
	my_avatar->pos.y = ntohl(msg_buff->avatar_turn.Pos[my_avatar->fd].y);
	pthread_mutex_lock(&my_turn);
	place_avatar(maze, my_avatar->pos.x, my_avatar->pos.y, my_avatar->fd);
	pthread_mutex_unlock(&my_turn);
    }

    //try to solve the maze
    while (ntohl(msg_buff->type) == AM_AVATAR_TURN) {
	//if it is your turn...
	if (ntohl(msg_buff->avatar_turn.TurnId) == my_avatar->fd) {
	    pthread_mutex_lock(&my_turn); //make sure no one else can go
			#ifdef SLEEP
				sleep(1); 
			#endif
	    make_move(maze, my_avatar, comm_sock, msg_buff);
	    pthread_mutex_unlock(&my_turn); //allow next person to go
	}
	else {
	    wait_for_response(comm_sock, msg_buff);
	}
    }

    // if loop has ended, either maze has been solved or an error occured
    // read the message buff to determine the return status
    switch (ntohl(msg_buff->type)) {
	case (AM_MAZE_SOLVED):
	    print_solved(maze);
	    ret_status = SUCCESS;
	    break;
	case (AM_TOO_MANY_MOVES):
	    ret_status = AM_TOO_MANY_MOVES;
	    break;
	case (AM_SERVER_TIMEOUT):
	    ret_status = AM_SERVER_TIMEOUT;
	    break;
	default:
	    if (IS_AM_ERROR(ntohl(msg_buff->type))) {
		ret_status = FAILED;
	    }
	    else {
		ret_status = UNCLEAR;
	    }
    }
    return ret_status;
}
