#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "logic.h"
#include "board.h"
#include "pos.h"

/* Creates the game according to the specifications provided in the command
   line. Requires three command line arguments: height, width, and run length. 
   These can be passed in any order, as long as they are properly labled with
   -h, -w, and -r. If the width value would result in ? column label(s), or if
   the arguments are not correct, an error message is raised. 

   @param int the number of arguments that are provided
   @param char** the array of arguments.  
   @return game* the game that is created from the command line arguments
   */
game* construct_game(int argc, char* argv[]) {
  if (argc != 8) {
    printf("The incorrect number of arguments were provided. Please start" 
                "a new game with the proper flags and values.\n");
    exit(1);
  }
  
  bool h_flag = false, w_flag = false, r_flag = false, b_flag = false;
  int h, w, r;
  enum type b;
  for (unsigned int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0 && i + 1 < argc) {
      h = atoi(argv[++i]);
      h_flag = true;
    } else if (strcmp(argv[i], "-w") == 0 && i + 1 < argc) {
      w = atoi(argv[++i]);
      w_flag = true;
    } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
      r = atoi(argv[++i]);
      r_flag = true;
    } else if (strcmp(argv[i], "-m") == 0) {
      b = MATRIX;
      b_flag = true;
    } else if (strcmp(argv[i], "-b") == 0) {
      b = BITS;
      b_flag = true;
    }else {
      printf("An invalid flag or argument was provided. Please start"
            " a new game with the valid arguments.\n");
      exit(1);
    }
  }

  if (!h_flag || !w_flag || !r_flag || !b_flag) {
    printf("Not all required flags were provided. Please start a new game with"
              " valid arguments\n");
    exit(1);
  }

  if (h < 1 || w < 1 || r < 1) {
    printf("Unplayable values for either the height, width, "
        "or run length were provided. Please start a new game with valid"
        " arguments.\n");
    exit(1);
  }

  if (w > 62) {
    printf("The width provided is too large for our display. Please start"
        " a new game with a valid width,\n");
    exit(1);
  }
  
  game* res = new_game(r, w, h, b);
  return res;
}

/* Attempts to drop a piece in a given game and in a given column. If the drop
   was succesful, the function returns true. Otherwise, the function informs 
   the player and returns false. 
   
   @param game* the given game that we are dropping in
   @param char the column that we are dropping a piece in
   @param char the label of the column that is dropped, used for error 
   messaging
   @return bool if the drop was sucessful 
   */
bool try_drop(game* g, char col, char input) {
  if (drop_piece(g, col)) {
    return true;
  } 
  printf("You may not drop a piece in column %c. Please enter a new "
            "move and try again.\n", input);
  return false;
}

/* Runs the main loop in the game. The game runs until an result is reached or
   an error is raised. The loop allows for inproper inputs of moves to be
   corrected with a new input. 

    @param game* the game that has been created
   */
void main_loop(game* g) {
  while (true) {
    board_show(g->b);
    
    char input;
    bool invalid_input = true;
    while (invalid_input) {
      switch (g->player) {
        case BLACKS_TURN:
          printf("Enter Black's move:  ");
          break;
        case WHITES_TURN:
          printf("Enter White's move:  ");
      }

      scanf("%c%*c", &input);

      if(input == '!' ) {
        if (offset(g)){
          invalid_input = false;
        } else {
          printf("An offset move is not possible with the current board. "
                    "Please enter a new input and try again.\n");
        }

      } else if (input == '^') {
        disarray(g);
        invalid_input = false;

      } else if ('0' <= input && input <= '9') {
        invalid_input = !try_drop(g, input - '0', input);

      } else if ('A' <= input && input <= 'Z') {
        invalid_input = !try_drop(g, 10 + input - 'A', input);

      } else if ('a' <= input && input <= 'z') {
        invalid_input = !try_drop(g, 36 + input - 'a', input); 
      } else {
        printf("That is an invalid input, please try again."); 
      }
    }
    
    printf("\n\n");
    

    char* game_state;
    switch (game_outcome(g)) {
      case DRAW:
        game_state = "draw";
        break;
      case WHITE_WIN:
        game_state = "win for white";
        break;
      case BLACK_WIN:
        game_state = "win for black";
        break;
      case IN_PROGRESS:
        continue;
    }

    printf("Game over! The game has resulted in a %s!\n", game_state);
    printf("The final state of the board is as follows: \n");
    board_show(g->b);
    printf("\n");
    printf("Thank you for completing a game of Topsy-Turvy. If you wish, "
                "please play again! \n");
    game_free(g);
    exit(1);
  }
}

int main(int argc, char* argv[]) {
  game* g = construct_game(argc, argv);

  printf("Welcome to Topsy-Turvy!\n");
  printf("The objective of the game is to complete a line of %u pieces in "
      "either a horizontal, vertical, or diagonal configuration.\n", g->run);
  printf("Players will alternate turns until this goal is reached. There are "
        "three types of moves: a drop, a disarray, and an offset.\n");
  printf("A drop works by dropping a piece into a chosen column, and the move "
      "may be made by entering the character label of the desired column.\n");
  printf("A disarray move flips the entire board, causing the pieces to fall "
      "in reverse order. A disarray move may be performed by entering the ^ "
      "character.\n");
  printf("An offset move removes the oldest piece of the current player and "
      "the newest piece of thier opponent. A move may be performed by entering"
      " the ! \ncharacter.\n");
  
  char input;
  printf("Black will start first. Please enter any character to start: ");
  scanf("%c", &input);

  main_loop(g);
}
