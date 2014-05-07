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
 ***********************************************************************/

#ifndef BOARD_H
#define BOARD_H


#define LIMIT 100                                    // Size of a string 
#define LEGEND_LEFT_BUFFER 19
#define LEGEND_RIGHT_BUFFER 40
#define BOARD_SIZE 9                                 // Size of the board
#define BLOCK_WIDTH 4                                // Width of block
#define BLOCK_HEIGHT 2                               // Height of block
#define Y_BUFFER BOARD_SIZE * BLOCK_HEIGHT + 7       // Buffer for board

													
extern char board[BOARD_SIZE][BOARD_SIZE];
extern char solved[BOARD_SIZE][BOARD_SIZE];
extern int set_pieces[BOARD_SIZE][BOARD_SIZE];
extern int error[BOARD_SIZE][BOARD_SIZE];

// enum to hold difficulty level
typedef enum { EASY, MEDIUM, HARD, EVIL } DIFFICULTY;


void print_legend(int row, int col, DIFFICULTY difficulty, int save);
void print_board(int row, int col);
void print_nums(int row, int col);
void print_inverted_block(int row, int col, char ch);
void mask_board(int row, int col);
void print_text_board();


#endif
