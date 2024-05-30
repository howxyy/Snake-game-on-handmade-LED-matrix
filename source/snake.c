#include "board.h"
#include "matrix.h"
#include "joystick.h"
#include "buzzer.h"
#include "system_tick.h"
#include "snake.h"
#include "doublelinkedlist.h"
#include <time.h>

color_e snake_matrix[MATRIX_SIZE][MATRIX_SIZE];

// Direction enum
typedef enum {
    RIGHT = 0, LEFT = 1, UP = 2, DOWN = 3
} Direction;

// Initialize the head of the doubly linked list
struct Node* head = NULL;

// Initialize the food
uint8_t foodx;
uint8_t foody;
uint8_t foodColor;

// Set starting direction

Direction currentDirection = RIGHT;
Direction nextDirection = RIGHT;

// Function to move the snake
void movement(){
    // Get the head node of the snake
	uint8_t length = getLength(head);
    struct Node* node = getNodeByIndex(head, length - 1);
    uint8_t row = node->row;
    uint8_t col = node->col;

    // Move the snake based on the direction
    switch (nextDirection)
    {
        case UP:
            // If the snake is not the edge of the matrix, move normally
            if (row < 7){
                insertEnd(&head, row + 1, col);
            }
            // If the snake is on the edge of the matrix, loop around
            else if (row == 7){
                insertEnd(&head, 0, col);
            }
            break;
        case DOWN:
            if (row > 0){
                insertEnd(&head, row - 1, col);
            }
            else if (row == 0){
                insertEnd(&head, 7, col);
            }
            break;
        case LEFT:
            if (col > 0){
                insertEnd(&head, row, col - 1);
            }
            else if (col == 0){
                insertEnd(&head, row, 7);
            }
            break;
        case RIGHT:
            if (col < 7){
            	insertEnd(&head, row, col + 1);
            }
            else if (col == 7){
                insertEnd(&head, row, 0);
            }
            break;
        default:
            break;
    }

    // Update the current direction with the next direction
    currentDirection = nextDirection;
    // Remove the old head (front) of the snake to simulate movement
    deleteHead(&head);
}
// Function spawn the food
void spawn_food(){
	uint8_t row;
	uint8_t col;
	uint8_t occupied[8][8]={{0},{0}};
	uint8_t length = getLength(head);
    // Mark occupied positions based on the snake's body
    for (uint8_t counter = 0; counter < length; counter++)
    {
	   struct Node* node = getNodeByIndex(head, counter);
       occupied[node->row][node->col] = 1;
    }

    // Generate the seed for randomness by reading the counter that is being incremented constantly
    srand(g_counter);

    // Check and select a new food position that is not occupied by the snake

    do{
    	row = rand() % 8;
    	col = rand() % 8;
    }
    while(occupied[row][col] == 1);
    // Update the food's position
    foodx=row;
    foody=col;
    // Choose a random color for the food
    foodColor = rand() % 5;
}
// Function update the matrix
void update_matrix(){
	// Turn off the buzzer indicating eaten food
	buzzer_set(off);
    // Clear the matrix by turning off every LED
	for (uint8_t i = 0; i < 8; i++){
		for (uint8_t y = 0; y < 8; y++){
	        snake_matrix[i][y] = NONE;
		}
	    }

    // Iterate through the doubly linked list representing the snake's body
    uint8_t length = getLength(head);
    for (uint8_t counter = 0; counter < length; counter++){
        // Set the corresponding LED in the matrix to represent the snake's body
        struct Node* node = getNodeByIndex(head, counter);
        snake_matrix[node->row][node->col]=GREEN;
    }
    // Set the corresponding LED in the matrix to represent the snake's head
    struct Node* snakeHead = getNodeByIndex(head, length - 1);
    snake_matrix[snakeHead->row][snakeHead->col]=BLUE;

    // Update the matrix with the position of the food and a random color
    switch(foodColor){
    	case 0:
    		snake_matrix[foodx][foody]=WHITE;
    		break;
    	case 1:
    		snake_matrix[foodx][foody]=PURPLE;
    		break;
    	case 2:
    		snake_matrix[foodx][foody]=TEAL;
    		break;
    	case 3:
    		snake_matrix[foodx][foody]=RED;
    		break;
    	case 4:
    		snake_matrix[foodx][foody]=LIGHTGREEN;
    		break;
    	default:
    		snake_matrix[foodx][foody]=PURPLE;
    		break;
    }
}
// Function to start the game
void start_game(){
	while(1){
		// Clear the matrix by turning off every LED
		for (uint8_t i = 0; i < 8; i++){
					for (uint8_t y = 0; y < 8; y++){
				        snake_matrix[i][y] = NONE;
					}
				    }
		 if (joystick_button() == 0){
		 		 break;
		 }
	}
	// Set starting direction
	nextDirection = RIGHT;

    // Initialize the snake with initial positions
    insertEnd(&head, 1, 2);
    insertEnd(&head, 1, 3);
    insertEnd(&head, 1, 4);

    // Generate the food
    spawn_food();
}
// Function to indicate game over
void game_over(){
	// Turn on the buzzer indicating game over
	buzzer_set(on);
	// Blink the matrix 10 times for 100ms
	for (int blink = 0; blink < 10; blink++){
	        matrix_draw(snake_matrix);
	        system_tick_delay_ms(100);
	    }
	// Turn off the buzzer indicating game over
	buzzer_set(off);

	// Delete the current snake
	uint8_t length = getLength(head);
	for (uint8_t counter = 0; counter < length; counter++){
	        deleteHead(&head);
	    }
}

// Function to check the consequences of the snake's next move
void check_move(){
    // Get the head of the snake
    uint8_t length = getLength(head);
    struct Node* snakeHead = getNodeByIndex(head, length - 1);
    struct Node* node = NULL;
    uint8_t row = snakeHead->row;
    uint8_t col = snakeHead->col;

    // Check the outcome of the next move
    for (uint8_t counter = 0; counter < length; counter++){
        node = getNodeByIndex(head, counter);
        // Snake moves without colliding with anything
        if (counter == (length - 1)){
            continue; // Skip the the head during checking
        }
        // If the next step is food, add a new node to the snake, generate a new random food and turn on the buzzer
        else if ((row == foodx) && (col == foody)){
            insertEnd(&head, row, col);
            spawn_food();
            buzzer_set(on);
        }
        // If the next step is the snake's body, indicate game over
        else if ((row == node->row) && (col == node->col)){
            game_over();
            start_game();
        }
    }
}

// Function to check for input from the joystick
void joystick_input (joystick_e input){
	 switch(input){
	 case (joystick_up):
		if (currentDirection != DOWN)
		    nextDirection = UP; // Change the snake's direction to "up" if not currently moving "down"
	 	 	break;
	 case (joystick_down):
		if (currentDirection != UP)
		    nextDirection = DOWN; // Change the snake's direction to "down" if not currently moving "up"
	 	 	break;
	 case (joystick_left):
		if (currentDirection != RIGHT)
			nextDirection = LEFT; // Change the snake's direction to "left" if not currently moving "right"
		    break;
	 case (joystick_right):
		if (currentDirection != LEFT)
		    nextDirection = RIGHT; // Change the snake's direction to "right" if not currently moving "left"
	 	 	break;
	 default:
		 	break;
	 }
}
