#ifndef BOARD_H
#define BOARD_H

#include "pos.h"


enum cell {
    EMPTY,
    BLACK,
    WHITE
};

typedef enum cell cell;


union board_rep {
    enum cell** matrix;
    unsigned int* bits;
};

typedef union board_rep board_rep;

enum type {
    MATRIX, BITS
};


struct board {
    unsigned int width, height;
    enum type type;
    board_rep u;
};

typedef struct board board;

/* Creates a new, fully-empty board of a given width and height. A particular
   type of board representation, either matrix or bits, but we will limit
   the representation to a matrix temporarily. If a bit representation is 
   requested, we will raise an error. Note that we mean that fully empty as
   a 2d array of cells that are set to the state EMPTY.

   @param unsigned int the number of columns in the board
   @param unsigned int the number of rows in the board
   @param enum type the representation for the board
   @return board* a pointer to the board that has been created
   */
board* board_new(unsigned int width, unsigned int height, enum type type);

/* Completely deallocates a passed board, including whichever internal 
   representation it is using. The function raises an error if the board
   claims to not use the matrix representation. 

   @param board* the board that we are deallocating
   */
void board_free(board* b);

/* Prints the passed board to the screen, along with row and column headers. 
   Specifically, the row headers will be to the left of the board, with one
   space to the left of the board. The column headers will be displayed one
   lines above the board, two spaces shifted to the right. Note that there
   may be more than 10 rows or columns. In this case, labels continue to count
   using capital letters, then with lowercase letters. If the lables continue
   after that, they will be filled with the '?' character. A cell that has
   an EMPTY value will be represented with the character '.'. A cell that 
   is has the value BLACK will be represented with the character '*'.
   A cell that has the value WHITE will be represented with the character 'o'.

   @param board* the board we are visually representing. 
   */
void board_show(board* b);

/* Retrives a cell in a given board at a provided position. 

   @param board* the board that we are pulling positions from
   @param pos the position that we wish to retrieve the corresponding cell from
   @return cell the cell at the designated position on the board
   */
cell board_get(board* b, pos p);

/* Modifies the cells within a given board at a given position. 

   @param board* the board we are observing cells and positions from.
   @param pos the position on the board we are considering
   @param cell the new value of the cell at the designated position
   */
void board_set(board* b, pos p, cell c);

#endif /* BOARD_H */
