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
 * sudoku.c - main part of the program, handles controls for the game  *
 *            as well as initialization and command line arguments.    *
 *                                                                     *
 ***********************************************************************/


#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include "sudoku.h"


int main(int argc, char *argv[]) {

	// Check for command line arguments other than the call to the
	// program itself
	if (argc > 1) {

		FILE *puzzle;

		if (strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--solve") == 0) {

			if ( (puzzle = fopen(argv[2], "r")) != NULL) {

				int i, j;
				// Scan in the file to the board array
				for (i = 0; i < BOARD_SIZE; i++) {
					for (j = 0; j < BOARD_SIZE - 1; j++) {
						fscanf(puzzle, "%c, ", &board[i][j]);
						
						if (board[i][j] == '0') {
							board[i][j] = ' ';
						}
					}
					fscanf(puzzle, "%c\n", &board[i][BOARD_SIZE - 1]);
					
					if (board[i][j] == '0') {
						board[i][j] = ' ';
					}
				}

				// Print the initial puzzle
				puts("");
				printf("\t\tInput Board");
				puts("");
				print_text_board();
				
				printf("\t\tSolved Board");
				puts("");
				backtrack(0, 0);
				print_text_board();

			} else {
				puts("Could not parse the puzzle file!");
				puts("Columns in the puzzle files should be comma delimited");
				puts("Rows should be separated by newlines, for example:");
				puts("");
				puts("7, 6, 0, 0, 0, 0, 0, 0, 0");
				puts("0, 8, 5, 6, 4, 0, 0, 0, 0");
				puts("2, 9, 4, 0, 8, 0, 0, 3, 0");
				puts("4, 0, 7, 0, 0, 8, 0, 0, 0");
				puts("0, 0, 0, 5, 0, 4, 0, 0, 0");
				puts("0, 0, 0, 1, 0, 0, 4, 0, 6");
				puts("0, 0, 0, 0, 7, 0, 8, 2, 1");
				puts("0, 0, 0, 0, 2, 9, 3, 6, 0");
				puts("0, 0, 0, 0, 0, 0, 0, 4, 9");
			}
			
		} else if (strcmp(argv[1], "-h") == 0  || strcmp(argv[1], "--help") == 0) {
			print_help();
		} else {
			printf("sudoku: illegal option -- \"%s\"\n", argv[1]);
			print_help();
		}

		return 0;
	}


	/****************************
	 * ncurses mode begins here *
	 ****************************/


	int row, col;    // Variables to hold terminal size.

    // Start screen
    initscr();
    clear();
    noecho();
    cbreak();

    keypad(stdscr, true);    // give the screen the keyboard

    if (has_colors() == TRUE)   // Test if terminal has color
        start_color();

    getmaxyx(stdscr, row, col);

    // Check if terminal is of proper size
    if (row < MIN_HEIGHT || col < MIN_WIDTH) {
	    char term_size_err2[LIMIT], hei[LIMIT], wid[LIMIT];
	    sprintf(hei, "%d", MIN_HEIGHT);
	    sprintf(wid, "%d", MIN_WIDTH);
	    strcpy(term_size_err2, "Resize your terminal to at least ");
	    strcat(term_size_err2, hei);
	    strcat(term_size_err2, " x ");
	    strcat(term_size_err2, wid);
	    strcat(term_size_err2, " and try again.");
	    
        char term_size_err1[] = "Your terminal is too small!!!";
        mvprintw(row / 2 - 1, (col - strlen(term_size_err1)) / 2, "%s", term_size_err1);
        mvprintw(row / 2    , (col - strlen(term_size_err2)) / 2, "%s", term_size_err2);
        getch();
        endwin();
        return 0;
    }

    // Print title
    attron(A_BOLD);
    char title[] = "WELCOME TO SUDOKU!!!";
    mvprintw(1, (col - strlen(title))/2, "%s", title);
    attroff(A_BOLD);
    
    // Print and initialize the board
    DIFFICULTY difficulty = MEDIUM;
    int save = 1;
    print_legend(Y_BUFFER, col, difficulty, save);
    print_board(Y_BUFFER, col);
	init_board(difficulty);
	print_nums(Y_BUFFER, col);
    
    nodelay(stdscr, true);   // don't wait for keyboard
    
    int ch;
    int y = (Y_BUFFER - BOARD_SIZE * BLOCK_HEIGHT) / 2 + BLOCK_HEIGHT/2,
	    x = (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + BLOCK_WIDTH/2,
	    y_index = 0,
	    x_index = 0;
	
	move(y, x);    // Move the cursor to the top left corner block

	// constants for board limits
	const int y_min = y;
	const int y_max = y_min + 8 * BLOCK_HEIGHT;
	const int x_min = x;
	const int x_max = x_min + 8 * BLOCK_WIDTH;

	// time variables
	time_t start = time(NULL);
	int min = 0, tens = 0, sec;
	
    
    while (ch != 'q') {
	    
	    ch = getch();

	    // Print the time
	    sec = (time(NULL) - start);
	    tens = sec / 10;
	    min = sec / 60;
	    mvprintw((Y_BUFFER - BOARD_SIZE * BLOCK_HEIGHT) / 2 + 1, (col - BOARD_SIZE * BLOCK_WIDTH) / 2 + LEGEND_RIGHT_BUFFER + 5, "%2d:%d%d", min, tens%6, sec%10);

	    move(y, x);

	    if (ch == KEY_UP || ch == 'k') {
		    
		    if (y > y_min) {
			    y -= BLOCK_HEIGHT;
			    y_index--;
			    move(y, x);
		    }
		    
	    } else if (ch == KEY_DOWN || ch == 'j') {
		    
		    if (y < y_max) {
			    y += BLOCK_HEIGHT;
			    y_index++;
			    move(y, x);
		    }
		    
	    } else if (ch == KEY_LEFT || ch == 'h') {
		    
		    if (x > x_min) {
			    x -= BLOCK_WIDTH;
			    x_index--;
			    move(y, x);
		    }
		    
	    } else if (ch == KEY_RIGHT || ch == 'l') {
		    
		    if (x < x_max) {
			    x += BLOCK_WIDTH;
			    x_index++;
			    move(y, x);
		    }
		    
	    } else if ( (ch == 'x' || ch == KEY_BACKSPACE || ch == KEY_DC ) && !set_pieces[y_index][x_index] ) {
		    
		    board[y_index][x_index] = ' ';
		    print_nums(Y_BUFFER, col);
		    
		    move(y, x);
		    
	    } else if ( ch == 'A' && !set_pieces[y_index][x_index] ) {
		    
		    if(board[y_index][x_index] < '9') {
			    board[y_index][x_index]++;
			    print_nums(Y_BUFFER, col);
		    }
		    
		    move(y, x);
		    
	    } else if ( ch == 'X' && !set_pieces[y_index][x_index] ) {
		    
		    if(board[y_index][x_index] > '1') {
			    board[y_index][x_index]--;
			    print_nums(Y_BUFFER, col);
		    }
		    
		    move(y, x);
		    
	    } else if ( (ch <= '9' && ch >= '1')  && !set_pieces[y_index][x_index] ) {

		    board[y_index][x_index] = ch;
		    print_nums(Y_BUFFER, col);
		    
		    move(y, x);

	    } else if ( ch == 'd' ) {

		    if (difficulty == EASY) {
			    difficulty = EVIL;
		    } else if (difficulty == MEDIUM) {
			    difficulty = EASY;
		    } else if (difficulty == HARD) {
			    difficulty = MEDIUM;
		    } else if (difficulty == EVIL) {
			    difficulty = HARD;
		    }
		    
		    print_legend(Y_BUFFER, col, difficulty, save);
		    init_board(difficulty);
		    print_nums(Y_BUFFER, col);
		    
		    start = time(NULL);

		    move(y_min, x_min);
		    
	    } else if ( ch == 'D' ) {

		    if (difficulty == EASY) {
			    difficulty = MEDIUM;
		    } else if (difficulty == MEDIUM) {
			    difficulty = HARD;
		    } else if (difficulty == HARD) {
			    difficulty = EVIL;
		    } else if (difficulty == EVIL) {
			    difficulty = EASY;
		    }
		    
		    print_legend(Y_BUFFER, col, difficulty, save);
		    init_board(difficulty);
		    print_nums(Y_BUFFER, col);
		    
		    start = time(NULL);
		    
		    move(y_min, x_min);
		    
	    } else if ( ch == 'R' ) {
		    
		    print_legend(Y_BUFFER, col, difficulty, save);
		    init_board(difficulty);
		    print_nums(Y_BUFFER, col);
		    
		    start = time(NULL);
		    
		    move(y_min, x_min);
		    
	    } else if ( ch == 'S' ) {
	    } else if ( ch == 'V' ) {

		    if (save)
			    save = 0;
		    else
			    save = 1;
		    
		    print_legend(Y_BUFFER, col, difficulty, save);
		    
		    move(y, x);
	    }

	    refresh();
	    usleep(50000);    // sleep for a bit
    }

    
    getch();
    clear();
    endwin();
	
	return 0;
}


// print_help - prints the help sequence
void print_help() {
	printf("Sudoku %d.%d.%d (%d-%d-%d)\n", MAJOR, MINOR, REVISION, REV_YEAR, REV_MONTH, REV_DAY);
	puts("usage: sudoku [<options>]... [<file>]...");
	puts("");
	puts("options:");
	printf("  -h, --help\t%-20s\n", "show help");
	printf("  -s, --solve\t%-20s\n", "solves a given sudoku file");
}
