/*
* A structure file to hold the available information about the maze
*
* Created by Benji Hannam, 2016 for use in Team Core Dumped in a Maze for CS50
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/************************* global variables ******************/

/************************* local types **********************/
//A struct for each grid spot in the maze
typedef struct spot {
	//bools for the walls
	bool north;
	bool south;
	bool east;
	bool west;
	//bools for states of the spot
	bool avatar;
	bool dead;
	bool visited;
	//arrays for more information on the spots
	int avatar_number[10];
	int visited_by[10];
} spot_t;

// A structure to be used to keep track of the last move of every avatar
typedef struct avatar_move{
	int direction;
	int score;
} avatar_move;

/************************* global types *********************/

typedef struct mazestruct{
	spot_t* map[100][100];
	int height;
	int width;
	int num_avatars;
	int move_count;
	int number_leaders;
	avatar_move* last_move[10];
	int leader_array[10];
	int *crossed_with[10];
	FILE* fp;
	bool is_solved;
} mazestruct_t;


/********************** global functions *********************/

/**************** maze_new() ****************/
/*
* Takes in the height and width of the maze and the number of avatars playing the game.
* Allocates memory for the maze structure and generates the 2d array of the grid spots.
* returns a pointer the the new maze structure generated.
*/
mazestruct_t* maze_new(int height, int width, int num_avatars, FILE* fp){

	mazestruct_t *new_maze = malloc(sizeof(mazestruct_t));
	if(new_maze == NULL){
		printf("Error allocating memory to new maze struct, exiting..\n");
		exit(2);
	}

	//insert all the spots into the map
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){

			spot_t *new_spot = malloc(sizeof(spot_t));
			if(new_spot == NULL){
				printf("Error allocaitng memory to a new spot struct, exiting..\n");
				exit(2);
			}
			//initialise the variables in spot
			new_spot->avatar = false;
			new_spot->dead = false;
			new_spot->visited = false;
			//initialise the direction bools
			new_spot->east = false;
			new_spot->north = false;
			new_spot->south = false;
			new_spot->west = false;
			//initialise the array
			for(int i = 0; i < 10; i++){
				new_spot->avatar_number[i] = 0;
			}

			for(int i = 0; i < 10; i++){
				new_spot->visited_by[i] = 0;
			}
			new_maze->map[i][j] = new_spot;
		}
	}

	//insert the walls on the north edge
	for (int i = 0; i < width; i++){
		new_maze->map[i][0]->north = true;
	}

	//insert the walls on the south edge
	for (int i = 0; i < width; i++){
		new_maze->map[i][height - 1]->south = true;
	}
	//insert the walls on the east edge
	for (int i = 0; i < height; i++){
		new_maze->map[width -1][i]->east = true;
	}

	//insert the walls on the west edge
	for (int i = 0; i < height; i++){
		new_maze->map[0][i]->west = true;
	}

	//initialise the variables
	//new_maze->map = temp;
	new_maze->height = height;
	new_maze->width = width;
	new_maze->num_avatars = num_avatars;
	new_maze->move_count = 0;
	new_maze->number_leaders = num_avatars;
	new_maze->fp = fp;
	new_maze->is_solved = false;

	//initialise the array for last moves
	for (int i = 0; i < 10; i++){
		avatar_move *new_move = malloc(sizeof(avatar_move));
		if(new_move == NULL){
			printf("Error allocaitng memory to a new avatar_move struct, exiting..\n");
			exit(2);
		}
		new_move->direction = 8;
		new_move->score = -1;
		//initialise last_move
		new_maze->last_move[i] = new_move;
		//initialise leader array
		new_maze->leader_array[i] = i;
	}

	// initialize crossed_with array
	for (int i = 0; i < num_avatars; i++) {
		// allocate each avatar's connected array
		int *connected_pointer = malloc(num_avatars*sizeof(int));
		for (int j = 0; j < num_avatars; j++) {
			if (j == i)
				connected_pointer[j] = 1;
			else
				connected_pointer[j] = 0;
		}

		new_maze->crossed_with[i] = connected_pointer;
	}

	return new_maze;
}

/**************** maze_print() ****************/
/*
* Prints out the 2D representation of the maze.
* Prints from left to right and top to bottom going row by row.
* If there are two avatars on the same spot it will print the lowest numbers avatar.
*
*/
void maze_print(mazestruct_t *maze){

	printf("   ");

	//if the width of the maze is less that 45 make the width of each spot bigger
	if(maze->width < 45){
		for (int i = 0; i < maze->width; i ++){
			printf("  %-2d", i);
		}
		printf("\n");

		//print the top edge
		printf("   ");
		for (int j = 0; j < maze->width; j++){
			if(maze->map[j][0]->north){
				printf("+---");
			}
			//otherwise print nothing
			else{
				printf("+   +");
			}
		}
		printf("+");

		//loop through each row
		for (int i = 0; i < maze->height; i++){

			printf("\n");
			printf("%-3d", i);
			//print the east wall
			printf("|");
			// print the west walls or avatars
			for (int j = 0; j < maze->width; j++){

				if(maze->map[j][i]->avatar){
					for(int n = 0; n < 10; n++){
						if(maze->map[j][i]->avatar_number[n] == 1){
							printf(" %d ", n);
							break;
						}
					}
				}
				else if(maze->map[j][i]->dead){
					printf(" D ");
				}
				// else if(maze->map[j][i]->visited){
				// 	printf(" V ");
				// }
				else{
					printf("   ");
				}
				if(maze->map[j][i]->east){
					printf("|");
				}
				else{
					printf(" ");
				}
			}
			printf("\n   ");

			//print any south walls
			for (int j = 0; j < maze->width; j++){
				if(maze->map[j][i]->south){
					printf("+---");
				}
				//otherwise print nothing
				else{
					printf("+   ");
				}
			}
			printf("+");
		}
		printf("\n");
	}
	//if we have more than 45 columns in the maze then scale down the width of each spot by half
	else{

		for (int i = 0; i < maze->width; i ++){
			printf("%-2d", i);
		}
		printf("\n");

		//print the top edge
		printf("   ");
		for (int j = 0; j < maze->width; j++){
			if(maze->map[j][0]->north){
				printf("+-");
			}
			//otherwise print nothing
			else{
				printf("+ +");
			}
		}
		printf("+");

		//loop through each row
		for (int i = 0; i < maze->height; i++){

			printf("\n");
			printf("%-3d", i);
			//print the east wall
			printf("|");
			// print the west walls or avatars
			for (int j = 0; j < maze->width; j++){

				if(maze->map[j][i]->avatar){
					for(int n = 0; n < 10; n++){
						if(maze->map[j][i]->avatar_number[n] == 1){
							printf("%d", n);
							break;
						}
					}
				}
				else if(maze->map[j][i]->dead){
					printf("D");
				}
				else{
					printf(" ");
				}
				if(maze->map[j][i]->east){
					printf("|");
				}
				else{
					printf(" ");
				}
			}
			printf("\n   ");

			//print any south walls
			for (int j = 0; j < maze->width; j++){
				if(maze->map[j][i]->south){
					printf("+-");
				}
				//otherwise print nothing
				else{
					printf("+ ");
				}
			}
			printf("+");
		}
		printf("\n");
	}
}

/**************** did_x_visit() ****************/
/*
* Checks if a spot denoted by a direction has been
* Takes in the maze struct, x,y coordinates, the direction and an avatar id as parameters.
* returns true if the avatar has visited it otherwise returns false.
*/
bool did_x_visit(mazestruct_t *maze, int x, int y, int direction, int avatar_id){
	//west spot
	if(direction == 0 && x > 0){
		return (maze->map[x - 1][y]->visited_by[avatar_id] == 1);
	}
	//east spot
	else if(direction == 3 && (x < (maze->width - 1))){
		return (maze->map[x + 1][y]->visited_by[avatar_id] == 1);
	}
	//north spot
	else if(direction == 1 && (y > 0)){
		return (maze->map[x][y - 1]->visited_by[avatar_id] == 1);
	}
	//south spot
	else if(direction == 2 && (y < maze->height - 1)){
		return (maze->map[x][y + 1]->visited_by[avatar_id] == 1);
	}
	//check the current spot
	else if(direction == -1){
		return (maze->map[x][y]->visited_by[avatar_id] == 1);
	}
	//if we make it down here
	return false;
}

/**************** is_someone_here() ****************/
/*
* Checks if there is an avatar other than the one with the given id at the
* 	current spot dictated by x and y.
* Takes in the maze struct, x,y coordinates and the id of the avatar checking
* 	if another avatar is present.
* Returns the lowest avatar id if another avatar is on the spot
* 	and -1 if none is found.
*/
int is_someone_here(mazestruct_t *maze, int x, int y, int id){
	//check coords
	if(x > (maze->width - 1) || y > (maze->height - 1) || x < 0 || y < 0){
		printf("Coordinates are out of range\n");
		return -1;
	}
	else {
	    for(int i = 0; i < maze->num_avatars; i++){
		if (i != id && maze->map[x][y]->avatar_number[i] == 1){
		    return i;
		}
	    }
	    return -1;
	}
}

/**************** print_locations() ****************/
/*
* prints the locations of each avatar in the form avatar_id: (x, y);
*
*/
void print_locations(mazestruct_t *maze){
	fprintf(maze->fp, "Avatar locations: ");

	for(int x = 0; x < maze->width; x++){
		for(int y = 0; y < maze->height; y++){

			if(maze->map[x][y]->avatar){

				for(int i = 0; i < maze->num_avatars; i++){

					if(maze->map[x][y]->avatar_number[i] == 1){
						fprintf(maze->fp, "%d: (%d, %d); ", i, x, y);
					}
				}
			}
		}
	}

	fprintf(maze->fp, "\n");

}

/**************** place_avatar() ****************/
/*
* Takes in a pointer to a maze struct, a pair of x,y coords and the id of the avatar.
* Then marks the avatar at being in that spot referenced by the x,y coordinates provided.
* Also prints the entire maze structure to stdout.
*/
void place_avatar(mazestruct_t *maze, int x, int y, int avatar_id){
	//if we are within bounds
	if(x < maze->width && y < maze->width && x > -1 && y > -1){
		//mark the spot appropriately
		maze->map[x][y]->avatar = true;
		maze->map[x][y]->visited = true;
		maze->map[x][y]->visited_by[avatar_id] = 1;
		maze->map[x][y]->avatar_number[avatar_id] = 1;
		//print the update
		printf("\n********************************************************************************\n");
		printf("Inserted avatar %d at %d,%d.\n", avatar_id, x, y);
		fprintf(maze->fp, "Inserted avatar %d at %d,%d.\n", avatar_id, x, y);
		print_locations(maze);
		maze_print(maze);
		printf("********************************************************************************\n");
	}
	else{
		printf("Avatar location is off the maze.\n");
	}
}

/**************** insert_wall() ****************/
/*
* Tells the maze structure where a wall has been found.
* Takes in a pointer to a maze struct, a pair of x,y coordinates and a direction denoted by:
* 0 = west, 1 = north, 2 = south and 3 = west.
*/
void insert_wall(mazestruct_t *maze, int x, int y, int direction, int avatar_id){

	if(x > (maze->width - 1) || y > (maze->height - 1) || x < 0 || y < 0){
		printf("Coordinates are out of range\n");
		return;
	}

	//if not marking as dead
	if(!maze->map[x][y]->dead){
		printf("\n********************************************************************************\n");
	}

	if(direction == 0){
		//set the west wall and the east wall of the spot to the left
		maze->map[x][y]->west = true;
		if(x > 0){
			maze->map[x -1][y]->east = true;
		}
		if(!maze->map[x][y]->dead){
			printf("Inserted west wall at %d,%d.\n", x, y);
			fprintf(maze->fp, "Avatar %d ran into west wall at %d,%d.\n", avatar_id, x, y);
			print_locations(maze);
		}
	}
	if(direction == 1){
		//set the north wall and the south wall of the spot;
		maze->map[x][y]->north = true;
		if(y > 0){
			maze->map[x][y - 1]->south = true;
		}
		if(!maze->map[x][y]->dead){
			printf("Inserted north wall at %d,%d.\n", x, y);
			fprintf(maze->fp, "Avatar %d ran into north wall at %d,%d.\n", avatar_id, x, y);
			print_locations(maze);
		}
	}
	if(direction == 2){
		//set the south wall and the north wall of the spot below
		maze->map[x][y]->south = true;
		if(y < (maze->height - 1)){
			maze->map[x][y + 1]->north = true;
		}
		if(!maze->map[x][y]->dead){
			printf("Inserted south wall at %d,%d.\n", x, y);
			fprintf(maze->fp, "Avatar %d ran into south wall at %d,%d.\n", avatar_id, x, y);
			print_locations(maze);
		}
	}
	if(direction == 3){
		//set the east wall and the west wall of the spot to the right
		maze->map[x][y]->east = true;
		if(x < (maze->width - 1)){
			maze->map[x + 1][y]->west = true;
		}
		if(!maze->map[x][y]->dead){
			printf("Inserted east wall at %d,%d.\n", x, y);
			fprintf(maze->fp, "Avatar %d ran into east wall at %d,%d.\n", avatar_id, x, y);
			print_locations(maze);
		}
	}

	//if we arent marking it as dead
	if(!maze->map[x][y]->dead){
		maze_print(maze);
		printf("********************************************************************************\n");
	}

}

/**************** visited_spot() ****************/
/*
* Marks a spot as being visited by an avatar.
* Takes in a pointer to a maze struct, a pair of x,y coords and the id of the avatar that visited it.
*
*/
void visited_spot(mazestruct_t *maze, int x, int y, int avatar_number){

	if(x > (maze->width - 1) || y > (maze->height - 1) || x < 0 || y < 0){
		printf("Coordinates are out of range\n");
		return;
	}
	if(avatar_number < 10 && avatar_number > -1){
		maze->map[x][y]->visited_by[avatar_number] = 1;
	}
	else{
		printf("avatar number is not valid \n");
	}
}

/****************** is_dead() ***********************/
/*
 * returns whether or not the spot in the a given direction from
 * 	the spot at (x,y) is dead
 */
bool is_dead(mazestruct_t *maze, int x, int y, int direction) {
	//check coords
	if(x > (maze->width - 1) || y > (maze->height - 1) || x < 0 || y < 0){
		printf("Coordinates are out of range, cannot check wall.\n");
		return false;
	}
	//return if spot to west is dead
	if(direction == 0 && x > 0){

		return maze->map[x-1][y]->dead;
	}
	//return if spot to north is dead
	else if(direction == 1 && y > 0){

		return maze->map[x][y-1]->dead;
	}
	//return if spot to south is dead
	else if(direction == 2 && y < maze->height-1){

		return maze->map[x][y+1]->dead;

	}
	//return if spot to east is dead
	else if(direction == 3 && x < maze->width-1){

		return maze->map[x+1][y]->dead;
	}
	else if(direction == -1){
		return maze->map[x][y]->dead;
	}
	else{
		return false;
	}
}

/**************** insert_dead_spot() ****************/
/*
* Marks a spot as being dead.
* If there is no avatar on the spot draws four walls around it
* Takes in a pointer to a maze struct, a pair of x,y coords and the id of the avatar that visited it.
*
*/
void insert_dead_spot(mazestruct_t *maze, int x,int y, int avatar_id){

	if(x > (maze->width - 1) || y > (maze->height - 1) || x < 0 || y < 0){
		printf("Coordinates are out of range, could not insert dead spot.\n");
		return;
	}
	else{
		//mark as dead
		maze->map[x][y]->dead = true;
		//if there are no avatars still on the spot...
		if (!(maze->map[x][y]->avatar)) {
		    maze->map[x][y]->visited = false;
		    //add west wall
		    insert_wall(maze, x, y, 0, avatar_id);
		    //add north wall
		    insert_wall(maze, x, y, 1, avatar_id);
		    //add south wall
		    insert_wall(maze, x, y, 2, avatar_id);
		    //add east wall
		    insert_wall(maze, x, y, 3, avatar_id);
		}

	}

	printf("\n********************************************************************************\n");
	printf("Inserted dead spot at %d,%d.\n", x, y);
	maze_print(maze);
	printf("********************************************************************************\n");
}

/**************** check_wall() ****************/
/*
* Checks if we know for sure there is a wall on the side of a spot
* Takes in a pointer to a maze struct, a pair of x,y coords and a direction designated by:
* 0 = west, 1 = north, 2 = south and 3 = west.
* Returns true if there is a wall and false if there is not.
*/
bool check_wall(mazestruct_t *maze, int x, int y, int direction){

	//check coords
	if(x > (maze->width - 1) || y > (maze->height - 1) || x < 0 || y < 0){
		printf("Coordinates are out of range, cannot check wall.\n");
		return false;
	}
	//return east wall status
	if(direction == 0){

		return maze->map[x][y]->west;
	}
	//return north wall status
	else if(direction == 1){

		return maze->map[x][y]->north;
	}
	//return south wall status
	else if(direction == 2){

		return maze->map[x][y]->south;

	}
	//return west wall status
	else if(direction == 3){

		return maze->map[x][y]->east;
	}
	else{
		return false;
	}

}

/**************** is_someone_adjacent() ****************/
/*
* Checks if there is someone the adjacent spot dictated by the inputted direction.
* Takes in the maze struct, x,y coordinates and the direction as parameters.
* returns the lowest avatar id if someone is adjacent and -1 if noone is found.
*/
int is_someone_adjacent(mazestruct_t *maze, int x, int y, int direction){

	//check coords
	if(x > (maze->width - 1) || y > (maze->height - 1) || x < 0 || y < 0){
		printf("Coordinates are out of range\n");
		return -1;
	}

	//west spot
	if(direction == 0 && x > 0){
		//loop through and check for each avatar return -1 if none.
		for(int i = 0; i < maze->num_avatars; i++){
			 if (maze->map[x - 1][y]->avatar_number[i] == 1){
			 	return i;
			 }
		}
		return -1;
	}
	//east spot
	else if(direction == 3 && (x < (maze->width - 1))){

		for(int i = 0; i < maze->num_avatars; i++){
			 if (maze->map[x + 1][y]->avatar_number[i] == 1){
			 	return i;
			 }
		}
		return -1;
	}
	//north spot
	else if(direction == 1 && (y > 0)){

		for(int i = 0; i < maze->num_avatars; i++){
			 if (maze->map[x][y - 1]->avatar_number[i] == 1){
			 	return i;
			 }
		}
		return -1;
	}
	//south spot
	else if(direction == 2 && (y < maze->height - 1)){

		for(int i = 0; i < maze->num_avatars; i++){
			 if (maze->map[x][y + 1]->avatar_number[i] == 1){
			 	return i;
			 }
		}
		return -1;
	}
	else if(direction == -1){
		for(int i = 0; i < maze->num_avatars; i++){
			 if (maze->map[x][y]->avatar_number[i] == 1){
			 	return i;
			 }
		}
		return -1;
	}
	else{
		return -1;
	}
}

/**************** update_location() ****************/
/*
* Moves an avatar from one spot to another one.
* Takes in a maze struct, the inital x,y and the new x,y coordinates and the id of the avatar to move.
*
*/
void update_location(mazestruct_t *maze, int init_x, int init_y, int new_x, int new_y, int avatar_id){

	//check the initial coords
	if(init_x > (maze->width - 1) || init_y > (maze->height - 1) || init_x < 0 || init_y < 0){
		printf("Initial coordinates are out of range, did not update_location.\n");
		return;
	}

	//check the new coords
	if(new_x > (maze->width - 1) || new_y > (maze->height - 1) || new_x < 0 || new_y < 0){
		printf("New coordinates are out of range, did not update location.\n");
		return;
	}

	//update the initial spot
	maze->map[init_x][init_y]->avatar_number[avatar_id] = 0;
	//assume there are no more avatars at that spot
	maze->map[init_x][init_y]->avatar = false;
	//check if there are more and set the bool to true if so
	for(int i = 0; i < 10; i++){
		if(maze->map[init_x][init_y]->avatar_number[i] == 1){
			maze->map[init_x][init_y]->avatar = true;
			break;
		}
	}

	//update the new spot
	maze->map[new_x][new_y]->avatar = true;
	maze->map[new_x][new_y]->visited = true;
	maze->map[new_x][new_y]->visited_by[avatar_id] = 1;
	maze->map[new_x][new_y]->avatar_number[avatar_id] = 1;

	printf("\n********************************************************************************\n");
	printf("Moved avatar %d from %d,%d to %d,%d.\n", avatar_id, init_x, init_y, new_x, new_y);
	fprintf(maze->fp, "Avatar %d moved from %d,%d to %d,%d.\n", avatar_id, init_x, init_y, new_x, new_y);
	print_locations(maze);
	maze_print(maze);
	printf("********************************************************************************\n");
}

/**************** is_visited() ****************/
/*
* Checks if a nearby spot denoted by the direction has been visited.
* returns true if it has, false if it hasn't
*/
bool is_visited(mazestruct_t *maze, int x, int y, int direction){

	//west spot
	if(direction == 0 && x > 0){
		return maze->map[x - 1][y]->visited;
	}
	//east spot
	else if(direction == 3 && (x < (maze->width - 1))){
		return maze->map[x + 1][y]->visited;
	}
	//north spot
	else if(direction == 1 && (y > 0)){
		return maze->map[x][y - 1]->visited;
	}
	//south spot
	else if(direction == 2 && (y < maze->height - 1)){
		return maze->map[x][y + 1]->visited;
	}
	//own spot
	else if(direction == -1){
		return maze->map[x][y]->visited;
	}
	else{
		return false;
	}

}


/**************** who_visited() ****************/
/*
* Returns the lowest numbered of id of the an avatar who's path is adjacent but
* has not crossed paths with the current avatar.
* Returns -1 if no such avatar exist.
*/
int who_visited(mazestruct_t *maze, int x, int y, int direction, int avatar_id) {
	int *my_crossed_with = maze->crossed_with[avatar_id];
	for (int i = 0; i < maze->num_avatars; i++) {
		if (my_crossed_with[i] == 0) {
			if(did_x_visit(maze, x, y, direction, i)) {
				return i;
			}
		}
	}
	return -1;
}

/**************** delete_maze() ****************/
/*
* Deletes the maze preventing memory leaks.
*
*
*/
void delete_maze(mazestruct_t *maze){

	if(maze != NULL){
	    	//add one for the last move
		maze->move_count++;
		//print the total number of moves
		printf("TOTAL NUMBER OF MOVES: %d\n", maze->move_count);

		//go through each spot and free the memory associated with it
		for (int i = 0; i < maze->width; i++){
			for (int j = 0; j < maze->height; j++){
				if(maze->map[i][j] != NULL){
					free(maze->map[i][j]);
				}
			}
		}

		for(int m = 0; m < 10; m++){
			if(maze->last_move[m] != NULL){
				free(maze->last_move[m]);
			}
		}

		for(int n = 0; n < maze->num_avatars; n++){
			if(maze->crossed_with[n] != NULL){
				for(int j = (n + 1); j < maze->num_avatars; j++){
					if(maze->crossed_with[j] == maze->crossed_with[n]){
						maze->crossed_with[j] = NULL;
					}
				}
				free(maze->crossed_with[n]);
				maze->crossed_with[n] = NULL;
			}
		}


		free(maze);
	}
}

/**************** remove_leader() ****************/
/*
* Decrements the number of leaders by 1 only if the avatar was previously a leader
*
*/
void remove_leader(mazestruct_t *maze, int avatar_id){
	if(maze->leader_array[avatar_id] == avatar_id){
		maze->number_leaders--;
	}
}

/**************** insert_last_move() ****************/
/*
* updates the last move for an avatar
*
*/
void insert_last_move(mazestruct_t *maze, int direction, int score, int avatar_id){

	//check if a last move exists for that avatar then set
	if(maze->last_move[avatar_id] != NULL){
		maze->last_move[avatar_id]->score = score;
		maze->last_move[avatar_id]->direction = direction;
		maze->move_count++; //increment the move count
	}

}

/**************** get_last_direction() ****************/
/*
* Returns the last direction in which an avatar moved
*
*/
int get_last_direction(mazestruct_t *maze, int avatar_id){

	if(maze->last_move[avatar_id] != NULL){
		return maze->last_move[avatar_id]->direction;
	}
	else{
		return -1;
	}
}

/**************** get_last_score() ****************/
/*
* Returns the last score of the last move for an avatar
*
*/
int get_last_score(mazestruct_t *maze, int avatar_id){

	if(maze->last_move[avatar_id] != NULL){
		return maze->last_move[avatar_id]->score;
	}
	else{
		return -1;
	}
}

/**************** get_number_leaders() ****************/
/*
* Returns the number of leaders
*
*/
int get_number_leaders(mazestruct_t *maze){
	if(maze != NULL){
		return maze->number_leaders;
	}
	else{
		return -1;
	}
}

/**************** set_leader() ****************/
/*
* Sets the leader of the given avatar
*
*/
void set_leader(mazestruct_t *maze, int avatar_id, int leader_id){
	if(maze != NULL){
		for(int i = 0; i < maze->num_avatars; i++){
			if(maze->leader_array[i] == avatar_id){
				maze->leader_array[i] = leader_id;
			}
		}
	}
}

/**************** get_leader() ****************/
/*
* Gets the leader of the given avatar
*
*/
int get_leader(mazestruct_t *maze, int avatar_id){
	if(maze != NULL){

		return maze->leader_array[avatar_id];
	}
	else{
		return -1;
	}
}


/**************** cross_paths() ****************/
/*
* Takes in the the ids of two avatars and marks that anyone the first avatar has crossed with,
* the second has also crossed with and vice versa.
* returns true if all avatars have crossed paths with each other, false otherwise.
*/
bool cross_paths(int id1, int id2, mazestruct_t *maze) {
	bool check_all_crossed = true;
	int *id1_crossed_with = maze->crossed_with[id1];
	int *id2_crossed_with = maze->crossed_with[id2];

	// OR the crossed_with arrays of each target avatar because any avatar A who
	// crosses with avatar B who has also crossed with avatar C would mean each
	// avatar A, B, C would contain crossed_with arrays of “A, B, C”.
	if (id1_crossed_with != id2_crossed_with) {
		for (int i = 0; i < maze->num_avatars; i++) {
			// just update avatar1's crossed_with array
			if (!(id1_crossed_with[i] = id1_crossed_with[i] || id2_crossed_with[i])) {
				check_all_crossed = false;	// while computing result array, check if all are crossed
			}
		}
		// simply set avatar2's crossed_with array to point to the updated avatar1's
		maze->crossed_with[id2] = maze->crossed_with[id1];
		// dont forget about any other crossed_with paths that also point to avatar2 
		for (int i = 0; i < maze->num_avatars; i++) {
			if (maze->crossed_with[i] == id2_crossed_with) {
				maze->crossed_with[i] = maze->crossed_with[id1];
			}
		}
		free(id2_crossed_with);
	}

	return check_all_crossed;
}


/**************** have_paths_crossed() ****************/
/*
* Returns if the paths of the two leaders have crossed
*
*/
bool have_paths_crossed(mazestruct_t *maze){
	int first, second;
	if(maze != NULL){
		first = maze->leader_array[0];

		for(int i = 1; i < maze->num_avatars; i++){
			if(maze->leader_array[i] != first){
				second = maze->leader_array[i];
			}
		}

		for (int x = 0; x < maze->width; x++){
			for (int y = 0; y < maze->height; y++){

				if((maze->map[x][y]->visited_by[first] == 1) && (maze->map[x][y]->visited_by[second] == 1)){
					return true;
				}

			}
		}
		return false;
	}
	else{
		return false;
	}
}

/**************** print_solved() ****************/
/*
* Prints the maze solved message
*
*/
void print_solved(mazestruct_t *maze){

	if(!maze->is_solved){
		maze->is_solved = true;
		fprintf(maze->fp, "SOLVED THE MAZE in %d moves.\n", maze->move_count);
	}
}

/**************** get_height() ****************/
/*
* Returns the height of the maze
*
*/
int get_height(mazestruct_t *maze){
	return maze->height;
}

/**************** get_width() ****************/
/*
* Returns the width of the maze
*
*/
int get_width(mazestruct_t *maze){
	return maze->width;
}

/**************** has_avatar() ****************/
/*
* Returns if a spot has an avatar
*
*/
bool has_avatar(mazestruct_t *maze, int x, int y){
	return maze->map[x][y]->avatar;
}
