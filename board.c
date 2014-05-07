/************************************************************************************ 
 * Copyright (c) 2014, John Cooper Bell                                             *
 * All rights reserved.                                                             *
 *                                                                                  *
 * Redistribution and use in source and binary forms, with or without               *
 * modification, are permitted provided that the following conditions are met:      *
 *                                                                                  *
 * 1. Redistributions of source code must retain the above copyright notice, this   *
 *    list of conditions and the following disclaimer.                              *
 * 2. Redistributions in binary form must reproduce the above copyright notice,     *
 *    this list of conditions and the following disclaimer in the documentation     *
 *    and/or other materials provided with the distribution.                        *
 *                                                                                  *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND  *
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED    *
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR  *
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES   *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;     *
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND      *
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT       *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS    *
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                     *
 *                                                                                  *
 * The views and conclusions contained in the software and documentation are those  *
 * of the authors and should not be interpreted as representing official policies,  *
 * either expressed or implied, of the FreeBSD Project.                             *
 ************************************************************************************/

#include <time.h>
#include <ncurses.h>
#include "board.h"


// Array to hold the board state
char board[BOARD_SIZE][BOARD_SIZE];

// Array to hold the solved board
char solved[BOARD_SIZE][BOARD_SIZE];

// Array to hold members of the initial puzzle set
int set_pieces[BOARD_SIZE][BOARD_SIZE];

// Array to hold pieces currently in an incorrect position
int error[BOARD_SIZE][BOARD_SIZE];


// print_legend - prints the legend
void print_legend(int row, int col, DIFFICULTY difficulty, int save) {

	
	// Right side of legend
	attron(A_BOLD);
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 1, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%s", "Time:");
	attroff(A_BOLD);
	
	
	attron(A_BOLD);
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 3, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%s", "Difficulty:");
	attroff(A_BOLD);

	switch(difficulty) {

	case EASY:
		mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 4, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%c", '*');
		mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 5, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%c", ' ');
		mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 6, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%c", ' ');
		mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 7, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%c", ' ');
		break;
	case MEDIUM:
		mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 4, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%c", ' ');
		mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 5, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%c", '*');
		mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 6, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%c", ' ');
		mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 7, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%c", ' ');
		break;
	case HARD:
		mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 4, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%c", ' ');
		mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 5, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%c", ' ');
		mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 6, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%c", '*');
		mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 7, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%c", ' ');
		break;
	case EVIL:
		mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 4, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%c", ' ');
		mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 5, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%c", ' ');
		mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 6, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%c", ' ');
		mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 7, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%c", '*');
		break;
	}
	
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 4, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER + 2, "%s", "Easy");
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 5, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER + 2, "%s", "Medium");
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 6, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER + 2, "%s", "Hard");
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 7, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER + 2, "%s", "Evil");
	
	attron(A_BOLD);
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 9, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%s", "Save Board:");
	attroff(A_BOLD);

	switch(save) {
	case 0:
		mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 9, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER + 12, "%c", 'N');
		break;
	case 1:
		mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 9, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER + 12, "%c", 'Y');
		break;
	}

	attron(A_BOLD);
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 13, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%s", "Manipulation:");
	attroff(A_BOLD);
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 14, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%s", " A - increment");
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 15, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER, "%s", " X - decrement");
	
	
	// Left side of legend
	attron(A_BOLD);
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 1, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 - LEGEND_LEFT_BUFFER, "%s", "Game:");
	attroff(A_BOLD);
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 2, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 - LEGEND_LEFT_BUFFER, "%s", " q - quit");
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 3, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 - LEGEND_LEFT_BUFFER, "%s", " R - new game");
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 4, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 - LEGEND_LEFT_BUFFER, "%s", " V - toggle save");
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 5, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 - LEGEND_LEFT_BUFFER, "%s", " S - solve board");
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 6, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 - LEGEND_LEFT_BUFFER, "%s", " d - easier");
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 7, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 - LEGEND_LEFT_BUFFER, "%s", " D - harder");
	
	attron(A_BOLD);
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 9, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 - LEGEND_LEFT_BUFFER, "%s", "Movements:");
	attroff(A_BOLD);
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 10, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 - LEGEND_LEFT_BUFFER, "%s", " - arrows");
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 11, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 - LEGEND_LEFT_BUFFER, "%s", " - hjkl");
	
	attron(A_BOLD);
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 13, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 - LEGEND_LEFT_BUFFER, "%s", "Deletion:");
	attroff(A_BOLD);
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 14, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 - LEGEND_LEFT_BUFFER, "%s", " - x");
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 15, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 - LEGEND_LEFT_BUFFER, "%s", " - delete");
	mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 16, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 - LEGEND_LEFT_BUFFER, "%s", " - backspace");
	
}

// print_board - prints the sudoku board on the screen
void print_board(int row, int col) {

	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
	init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(4, COLOR_MAGENTA, COLOR_RED);

	
	// Draw the solid inner lines
	attron(COLOR_PAIR(1));
	
	mvhline((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 +     BLOCK_HEIGHT, (col - BOARD_SIZE * BLOCK_WIDTH) / 2, ACS_HLINE, BOARD_SIZE * BLOCK_WIDTH);
	mvhline((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 2 * BLOCK_HEIGHT, (col - BOARD_SIZE * BLOCK_WIDTH) / 2, ACS_HLINE, BOARD_SIZE * BLOCK_WIDTH);
	mvhline((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 4 * BLOCK_HEIGHT, (col - BOARD_SIZE * BLOCK_WIDTH) / 2, ACS_HLINE, BOARD_SIZE * BLOCK_WIDTH);
	mvhline((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 5 * BLOCK_HEIGHT, (col - BOARD_SIZE * BLOCK_WIDTH) / 2, ACS_HLINE, BOARD_SIZE * BLOCK_WIDTH);
	mvhline((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 7 * BLOCK_HEIGHT, (col - BOARD_SIZE * BLOCK_WIDTH) / 2, ACS_HLINE, BOARD_SIZE * BLOCK_WIDTH);
	mvhline((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 8 * BLOCK_HEIGHT, (col - BOARD_SIZE * BLOCK_WIDTH) / 2, ACS_HLINE, BOARD_SIZE * BLOCK_WIDTH);
	
	mvvline((row - BOARD_SIZE * BLOCK_HEIGHT) / 2, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 +     BLOCK_WIDTH, ACS_VLINE, BOARD_SIZE * BLOCK_HEIGHT);
	mvvline((row - BOARD_SIZE * BLOCK_HEIGHT) / 2, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + 2 * BLOCK_WIDTH, ACS_VLINE, BOARD_SIZE * BLOCK_HEIGHT);
	mvvline((row - BOARD_SIZE * BLOCK_HEIGHT) / 2, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + 4 * BLOCK_WIDTH, ACS_VLINE, BOARD_SIZE * BLOCK_HEIGHT);
	mvvline((row - BOARD_SIZE * BLOCK_HEIGHT) / 2, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + 5 * BLOCK_WIDTH, ACS_VLINE, BOARD_SIZE * BLOCK_HEIGHT);
	mvvline((row - BOARD_SIZE * BLOCK_HEIGHT) / 2, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + 7 * BLOCK_WIDTH, ACS_VLINE, BOARD_SIZE * BLOCK_HEIGHT);
	mvvline((row - BOARD_SIZE * BLOCK_HEIGHT) / 2, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + 8 * BLOCK_WIDTH, ACS_VLINE, BOARD_SIZE * BLOCK_HEIGHT);
	
	attroff(COLOR_PAIR(1));
	
	// Draw the outer box lines
	attron(A_BOLD | COLOR_PAIR(2));
	
	int i, j;
	for (j = 0; j <= 9; j += 3) {
		
		move((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + j * BLOCK_HEIGHT, (col - BOARD_SIZE * BLOCK_WIDTH) / 2);
		
		for (i = 0; i < BOARD_SIZE * BLOCK_WIDTH; i++) {
			if (!(i % BLOCK_WIDTH))
				addch('+');
			else
				addch('-');
		}
		
	}
	
	for (j = 0; j <= 9; j += 3) {
		
		for (i = 0; i < BOARD_SIZE * BLOCK_HEIGHT; i++) {
			move((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + i, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + j * BLOCK_WIDTH);
			if (!(i % BLOCK_HEIGHT))
				addch('+');
			else
				addch('|');
		}
		
	}
	
	for (i = 0; i <= 9; i += 3) {
		for (j = 0; j <= 9; j += 3) {
			move((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + i * BLOCK_HEIGHT, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + j * BLOCK_WIDTH);
			addch('+');
		}
	}
		
	attroff(A_BOLD | COLOR_PAIR(2));

}

// print_nums - prints the board array onto the board
void print_nums(int row, int col) {

	mask_board(row, col);
	
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {

			if (set_pieces[i][j]) {
				
				attron(A_BOLD | COLOR_PAIR(3));
				move((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + (2 * i + 1) * BLOCK_HEIGHT/2,
				     (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + (2 * j + 1)* BLOCK_WIDTH/2);
				
				addch(board[i][j] | A_BOLD);
				attroff(A_BOLD | COLOR_PAIR(3));

			} else if (error[i][j]) {
				
				print_inverted_block((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + (2 * i + 1) * BLOCK_HEIGHT/2,
				                     (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + (2 * j + 1)* BLOCK_WIDTH/2, board[i][j]);

			} else {
			
				move((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + (2 * i + 1) * BLOCK_HEIGHT/2,
				     (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + (2 * j + 1)* BLOCK_WIDTH/2);
				
				addch(board[i][j]);
			
			}
		}
	}
	
	// move to top left corner
	move((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + BLOCK_HEIGHT/2,
		 (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + BLOCK_WIDTH/2);
}

// print_inverted_block - prints an inverted colored block
void print_inverted_block(int row, int col, char ch) {
	
	attron(A_BOLD | COLOR_PAIR(4));

	mvprintw(row, col-1, "%c", ' ');
	mvprintw(row, col, "%c", ch);
	mvprintw(row, col+1, "%c", ' ');
	
	attroff(A_BOLD | COLOR_PAIR(4));
}

// mask_board - covers all board squares with blank spaces
void mask_board(int row, int col) {

	int i, j;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {

			mvprintw((row - BOARD_SIZE * BLOCK_HEIGHT) / 2 + (2 * i + 1) * BLOCK_HEIGHT/2,
				     (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + (2 * j + 1)* BLOCK_WIDTH/2 - 1, "%s", "   ");
				
		}
	}
}

// print_text_board() - prints a text version of the sudoku board
void print_text_board() {

    printf("\n");

    int i, j;
    printf("   ");
    for (i = 0; i < BOARD_SIZE; i++)
        printf("  %d ", i+1);
    printf("\n");


    for (i = 0; i < BOARD_SIZE; i++) {

        printf("   ");

        if (i == 0) {
	        for (j = 0; j < BOARD_SIZE; j++)
		        printf("+---");
	        printf("+\n");

        } else {
	        for (j = 0; j < BOARD_SIZE; j++)
		        printf("|---");
	        printf("|\n");
        }

        printf(" %d ", i+1);
        for (j = 0; j < BOARD_SIZE; j++) {
	        printf("| %c ", board[i][j]);
        }
        printf("|\n");
    }

    printf("   ");
    for (j = 0; j < BOARD_SIZE; j++)
        printf("+---");
    printf("+\n");

    printf("\n");
}
