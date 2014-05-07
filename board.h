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
