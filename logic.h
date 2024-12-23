#ifndef LOGIC_H
#define LOGIC_H

#include <stdbool.h>
#include "board.h"


enum turn {
    BLACKS_TURN,
    WHITES_TURN
};

typedef enum turn turn;


enum outcome {
    IN_PROGRESS,
    BLACK_WIN,
    WHITE_WIN,
    DRAW
};

typedef enum outcome outcome;


struct game {
    unsigned int run;
    board* b;
    posqueue *black_queue, *white_queue;
    turn player;
};

typedef struct game game;

/* Creates a new game with the specified size and configuration. It also 
   uses the desired data representation. The function raises an error if it 
   is not possible to complete at least one vertical, horizontal, or diagonal 
   run. If it is not, the function raises an error. Also note that the 
   starting board is an empty board. 

   @param unsigned int the number of cells in a row to make a run
   @param unsigned int the number of columns the board has
   @param unsigned int the number of rows the board has
   @param enum type the type of the representation the board should use
   @return game* a pointer to the new game that we have constructed.
   */
game* new_game(unsigned int run, unsigned int width,
               unsigned int height, enum type type);

/* Completely deallocates a given game struct. Note that the function relies
   on the board_free and posqueue_free functions intensively.

   @param game* a pointer to the game that we will deallocate.
   */
void game_free(game* g);

/* Drops a piece belonging to the play whose turn it is in a specified column.
   The piece is placed at the lowest open cell in the column. If the column
   is already full, no changes are made. If the piece is succesfully dropped,
   the respective queue is updated and the turn is swapped. If an illegal move
   is requested, again, no changes will be made. 

   @param game* the game that we are dropping a piece into
   @param column the column in which we are dropping the piece into
   @return bool is false if the column is completely full and or an illegal 
   was requested (when the function does nothing). Otherwise, it is true.
   */
bool drop_piece(game* g, unsigned int column);

/* Completes the move named disarray in the game. The move works by reflecting
   the current game board across the horizontal midline. Then, the piences at
   their new positions fall freely due to gravity, resting at the bottom of
   the game. The function implements this move by flipping the order of the
   pieces in each column. Also note that the positions of the queues has also 
   been updated to reflect each piece's change in position. After the move is
   completed, the player is changed to reflect the change in turn.

   @param game* the game that we are performing the disarray move on
   */
void disarray(game* g);

/* Performes the move named offset in a given game. The move works by removing
   the oldes piece of the current player and the newest piece of their 
   opponent, then updating the positions of any pieces above the removed
   pieces in their column(s). After the offset, both player's queues are 
   updated accordingly. Most of the time, the function will return true, but
   if one or more players have no pieces on the board, do nothing and return 
   false.

   @param game* the game that we are performing the offset move on
   @return bool if the function is able to complete a valid offset move
   */
bool offset(game* g);

/* Reports either the outcome of a completed game, or if that game is still
   in progress based on the current state of the board.

   @param game* the game that we are reporting the state of
   @return outcome the outcome of the game that we are analyzing
   */
outcome game_outcome(game* g);

#endif /* LOGIC_H */
