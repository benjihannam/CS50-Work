/*
 * avatar.h - header file for avatar.c
 * defines methods for the Avatar struct
 * holds all move-making logic
 *
 * Benjamin Littlejohn, August 2016
 * Team: core_dumped_in_a_maze
 */

/********************************** includes ******************************/
#include "amazing.h"
#include "mazestruct.h"
#include <netdb.h>

#ifndef _avatar_h_
#define _avatar_h_

/****************************** prototypes ********************************/
/****************************** make_move() *******************************/
/*
 * Method to move an avatar so it can solve the Maze Challenge
 *
 * Input:
 * 	maze - database shared betweeen all avatars storing what is currently
 * 	       known about the maze
 * 	avatar - avatar that is making a move
 * 	comm_sock - communication socket between avatar and server
 *	msg_buff - a meessage buffer to store messages to and from the server
 *
 * High level description: 
 *	1. Determine the best move
 *	2. Send the move to the server
 *	3. Wait for response from server
 *	4. Update global maze data structure accordingly so all avatars are aware
 *	   of the results of the move
 *
 * Algorithm for determining best move:
 * 	1. If there is a known route between all avatars...
 *  	   Each avatar follows that route to bring all the avatars together. 
 *	2. If a path connecting all avatars is not yet discovered...
 *	   Run through all possible moves to see what priorities each satisfies
 * 	   Choose the move that satisfies the highest priority
 *
 * Priorities before all avatars have crossed paths:
 * 	1. Crossing paths with an avatar you haven't crossed paths with
 * 	2. Visiting an unvisited space
 * 	3. Visiting a space that was visited by another avatar
 * 	4. Visiting a space I have already visited (backtrack); if this
 * 	   happens mark the route as leading to a dead end
 * 	5. Visit a spot that has been marked as leading to a dead end 
 * 	   by another avatar
 * 	6. Don't run into a known wall!!!
 *
 * Priorities after all avatars have crossed paths:
 * 	1. Join up with another avatar; from now on, the lowest number avatar 
 * 	   in the group is the leader and everyone else follows
 * 	2. Visit a spot visited by another avatar in hopes that you will 
 * 	   cross paths later
 * 	3. Visiting a space I have already visited (backtrack); if this
 * 	   happens mark the route as leading to a dead end
 * 	4. Ignore all unvisited spaces; they don't lead to avatars
 * 	5. And of course don't run into a known wall!!!
 */
void make_move(mazestruct_t *maze, Avatar *avatar, int comm_sock,
	AM_Message *msg_buff);

/*************************** wait_for_response() ****************************/
/*
 * Waits until the server sends a message then reads it into the given
 * 	message buffer
 *
 * Caller is responsible for allocating memory for the message buffer and
 * 	freeing it
 */
void wait_for_response(int comm_sock, AM_Message *msg_buff);

#endif
