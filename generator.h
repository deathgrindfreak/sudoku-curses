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

#include "board.h"


#ifndef GENERATOR_H
#define GENERATOR_H 

void init_board(DIFFICULTY difficulty);
void generate_solved_board();
void dig_board(DIFFICULTY difficulty);
int backtrack(int row, int col);
int in_quadrant(int i, int j, char num);
int is_full_board();
int is_in_row(int row, char num);
int is_in_col(int col, char num);
int is_legal(int row, int col, char num);

	
#endif
