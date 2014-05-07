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
#include "generator.h"


#ifndef SUDOKU_H
#define SUDOKU_H

// Constants for version and whatnot
#define MAJOR 0
#define MINOR 0
#define REVISION 1
#define REV_YEAR 2014
#define REV_MONTH 4
#define REV_DAY 2

#define MIN_WIDTH BLOCK_WIDTH * BOARD_SIZE + 9      // Minimum width
#define MIN_HEIGHT BLOCK_HEIGHT * BOARD_SIZE + 10    // Minimum height


void print_help();
	

#endif
