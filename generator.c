/***********************************************************************
 *                                                                     *    
 * Name: Cooper Bell                                                   *    
 * Class: COSC 2420                                                    *    
 * Due Date:                                                           *    
 *                                                                     *    
 * Goal: Make a functioning Sudoku game using the ncurses programming  *
 *       library to generate the CLI.  The game should also include a  *
 *       "solver", that as its name suggests, solves a Sudoku puzzle   *
 *       of any difficulty.                                            *
 *                                                                     *
 *                                                                     *
 * generator.c - holds all functions responsible for generating and    *
 *               solving sudoku puzzles.                               *
 *                                                                     *
 ***********************************************************************/

#include <time.h>
#include <stdlib.h>
#include <ncurses.h>
#include "generator.h"

#include <unistd.h>  // Testing only

// init_board - initializes the board to a new play
void init_board(DIFFICULTY difficulty) {

	int i, j;

	// Initialize all board state arrays
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			board[i][j] = ' ';
			solved[i][j] = ' ';
			set_pieces[i][j] = 0;
			error[i][j] = 0;
		}
	}
	
	// Generate a random completely solved board
	generate_solved_board();

	
	// Copy the solved board into the solved array
	for (i = 0; i < BOARD_SIZE; i++)
		for (j = 0; j < BOARD_SIZE; j++)
			solved[i][j] = board[i][j];
	
	
	//dig_board(MEDIUM);
	

	for (i = 0; i < BOARD_SIZE; i++)
		for (j = 0; j < BOARD_SIZE; j++)
			if (board[i][j] != ' ')
				set_pieces[i][j] = 1;
}

// generate_solved_board - generates a completed board
void generate_solved_board() {
	
	srand(time(NULL));

	int i, x, y;

	// Choose a random block
	int x_block_offset = 3 * (rand() % 3);
	int y_block_offset = 3 * (rand() % 3);
	
	// choose a random row that intersects the block
	int x_offset = rand() % 3;
	
	// choose a random column that intersects the block
	int y_offset = rand() % 3;

	// Fill block randomly with digits 1-9
	for (i = 0; i < BOARD_SIZE; i++) {

		do { // Fill upper left block with random 1-9 digits
			x = rand() % 3 + x_block_offset;
			y = rand() % 3 + y_block_offset;
		} while (board[y][x] != ' ');
		
		board[y][x] = '0' + i + 1;
	}

	// Fill row with numbers not in first block
	for (i = 0; i < BOARD_SIZE; i++) {
		if (!is_in_row(y_block_offset + y_offset, i + 1 + '0')) {
			do {
				x = rand() % 9;
			} while (board[y_block_offset + y_offset][x] != ' ');
			
			board[y_block_offset + y_offset][x] = '0' + i + 1;
		}
	}
	// Fill column with numbers not in first block
	for (i = 0; i < BOARD_SIZE; i++) {
		if (!is_in_col(x_block_offset + x_offset, i + 1 + '0')) {
			do {
				y = rand() % 9;
			} while (board[y][x_block_offset + x_offset] != ' ');
			
			board[y][x_block_offset + x_offset] = '0' + i + 1;
		}
	}

	// Start backtracking to finish filling the board
	backtrack(0, 0);
}


// dig_board - digs out a board and grades to a certain difficulty
void dig_board(DIFFICULTY difficulty) {

	print_text_board();
	sleep(1);
	
	srand(time(NULL));

	// Temp array to hold the state of the board
	char temp[BOARD_SIZE][BOARD_SIZE];

	int i, j, blank_count = 0;
	
	// Copy board into temp
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			
			temp[i][j] = board[i][j];
			
			if (board[i][j] == ' ')
				blank_count++;
		}
	}
			
	if (blank_count < 27) {

		// Find a random non-empty spot on the board
		int row, col;
		do {
			row = rand() % BOARD_SIZE;
			col = rand() % BOARD_SIZE;
		} while (board[row][col] == ' ');

		
		// Temporarily set the cell to empty
		board[row][col] = ' ';
		
		// If the board is able to be solved keep the cell empty
		int back;
		if ((back = backtrack(0, 0))) {

			// Copy temp back into board
			for (i = 0; i < BOARD_SIZE; i++)
				for (j = 0; j < BOARD_SIZE; j++)
					board[i][j] = temp[i][j];

			// Empty the cell
			board[row][col] = ' ';
			printf("balls\n");
			
		} else {
			
			// Copy temp back into board, leaving the cell filled
			for (i = 0; i < BOARD_SIZE; i++)
				for (j = 0; j < BOARD_SIZE; j++)
					board[i][j] = temp[i][j];
			
		}
		
		printf("backtrack: %d\n", back);
		
		// Keep digging...
		dig_board(difficulty);
		
	} else {
		return;
	}
}


// backtrack - completes the filling of the board using recursive backtracking
int backtrack(int row, int col) {
	
	// If the board is full, then we have a solution!!!
	if (is_full_board())
		return 1;


	if (board[row][col] != ' ') {
		if (col == BOARD_SIZE - 1) {
			if (backtrack(row + 1, 0))
				return 1;
		} else {
			if (backtrack(row, col + 1))
				return 1;
		}
	}

	int i;

	// Iterate through all possible sudoku numbers
	for (i = 0; i < BOARD_SIZE; i++) {
		if (is_legal(row, col, i + 1 + '0')) {
				
			// Temporarily assign i to the board
			board[row][col] = i + 1 + '0';

			// Check recursively if i results in a solved board 
			if (col == BOARD_SIZE - 1) {
				if (backtrack(row + 1, 0))
					return 1;
			} else {
				if (backtrack(row, col + 1))
					return 1;
			}
		}
	}
		
	// At the is point none of the numbers will result in a solved
	// board, thus we must initiate backtracking
	board[row][col] = ' ';
	return 0;
}

// in_quadrant - checks if a number at i, j is in the quadrant
int in_quadrant(int i, int j, char num) {

	// Find the quadrant num resides in 
	int row_start = 0, row_end = 0, col_start = 0, col_end = 0;

	while ( !( i >= row_start && i <= row_start + 2 ) )
		row_start += 3;
	
	while ( !( j >= col_start && j <= col_start + 2 ) )
		col_start += 3;
	
	row_end = row_start + 2;
	col_end = col_start + 2;

	
	// See if num is already present in quadrant
	int row, col;
	for (row = row_start; row <= row_end; row++)
		for (col = col_start; col <= col_end; col++)
			if (board[row][col] == num)
				return 1;
	
	return 0;
}

// is_full_board - determines if the board is filled
int is_full_board() {

	int row, col;
	for (row = 0; row < BOARD_SIZE; row++)
		for (col = 0; col < BOARD_SIZE; col++)
			if (board[row][col] == ' ')
				return 0;
	return 1;
}

// is_in_row - determines if num is present in a row
int is_in_row(int row, char num) {

	int col;
	for (col = 0; col < BOARD_SIZE; col++)
		if (board[row][col] == num)
			return 1;
	return 0;
}

// is_in_col - determines if num is present in a column
int is_in_col(int col, char num) {

	int row;
	for (row = 0; row < BOARD_SIZE; row++)
		if (board[row][col] == num)
			return 1;
	return 0;
}

// is_legal - determines if move is legal (not in row, column or quadrant)
int is_legal(int row, int col, char num) {

	return !(is_in_col(col, num) ||
	         is_in_row(row, num) || 
	         in_quadrant(row, col, num));
}	
