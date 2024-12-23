#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "logic.h"

game* new_game(unsigned int run, unsigned int width,
               unsigned int height, enum type type) {
  if (run > height && run > width) {
    fprintf(stderr, "new_game, not possible to make run in board\n");
    exit(1);
  } 

  game* res = (game*) malloc (sizeof(game));
  if (!res) {
    fprintf(stderr, "new_game, unable to allocate result");
    exit(1);
  }

  res->run = run;
  res->player = BLACKS_TURN;
  res->b = board_new(width, height, type);
  res->black_queue = posqueue_new();
  res->white_queue = posqueue_new();
  return res;
}

void game_free(game* g) {
  board_free(g->b);
  posqueue_free(g->black_queue);
  posqueue_free(g->white_queue);
  free(g);
}

bool drop_piece(game* g, unsigned int column){
  if (column >= g->b->width) {
    return false;
  }

  for (int r = g->b->height - 1; r >= 0; r--) {
    pos p = {r, column};
    if (board_get(g->b, p) == EMPTY) {
      switch (g->player) {
        case BLACKS_TURN:
          board_set(g->b, p, BLACK);
          pos_enqueue(g->black_queue, p);
          break;
        case WHITES_TURN:
          board_set(g->b, p, WHITE);
          pos_enqueue(g->white_queue, p);
          break;
      }
      g->player = (g->player + 1) % 2;
      return true;
    }
  }
  return false;
}
/* Updates the position queues to reflect the changes made by disarray. The 
   function only loops through the queue once, relying on a list of the number
   of pieces in each column, refered to by the array col_height.

   @param posqueue* the queue that we are altering
   @param unsigned int the height of the board that we have altered, used for 
   calculations
   @param unsigned int* the list of the number of pieces in each column
   */
void disarray_update_queue(posqueue* q, unsigned int height, 
                                        unsigned int* col_height) {
  pq_entry* current = q->head;
  while (current) {
    pos old_p = current->p;
    pos new_p = {old_p.r, old_p.c};
    // finding the middle index of the stack of pieces in a column
    // note: avoided using floats by multiplying by 2
    unsigned int mid_i = 2 * height - (col_height[old_p.c] - 1) - 2;
    if (2 * old_p.r < mid_i) {
      //performing the swap if it got swapped down
      new_p.r = (height-1) - (old_p.r - (height - col_height[old_p.c]));
      //bottom index - (old index - column start index)
    } else if (2 * old_p.r > mid_i) {
      //performing the swap if it got swapped up
      new_p.r = (height - col_height[old_p.c]) + (height - 1 - old_p.r);
      //column start index + index of cell from bottom
    }
    current->p = new_p;

    current = current->next;
  }
}

/* Runs disarray for a single column by swapping the pieces in the column. The
   function does so by running a flipping algorithm. It also stores the height
   of the column into an array of column heights. 

   @param game* the game that we are performing the disarray on
   @param unsigned int the column that we are altering
   @param unsigned int* the array of column heights
   */
void disarray_single_column(game* g, unsigned int c, unsigned int* col_h) {
  unsigned int r;
  for (r = 0; r < g->b->height; r++) {
    if (board_get(g->b, make_pos(r,c)) != EMPTY){
      break;
    } 
  }
  col_h[c] = g->b->height - r;
  for (unsigned int i = 0; i < col_h[c] / 2; i++){
    pos p1 = {r+i, c};
    pos p2 = {g->b->height - i - 1, c};
    cell temp = board_get(g->b, p1);
    board_set(g->b, p1, board_get(g->b, p2));
    board_set(g->b, p2, temp);
  }
}

struct args {
  unsigned int c;
  game* g;
  unsigned int* col_height;
};

/* Wrapper function for the pthread call. Calls disarray on a single column
   and takes in an args struct. 

   @param void* an arguments struct. 
   @return void* returns NULL always, as disarray is a void function
   */
void* disarray_wrapper(void* a) {
  struct args* args = (struct args*) a;
  unsigned int c = args->c;
  game* g = args->g;
  unsigned int* col_height = args->col_height;

  disarray_single_column(g, c, col_height);

  return NULL;
}

void disarray(game* g) {
  unsigned int* col_height = (unsigned int*) malloc 
                                    (sizeof(unsigned int)*g->b->width);
  if (!col_height) {
    fprintf(stderr, "disarray, unable to allocate column heights\n");
    exit(1);
  }
  pthread_t threads[g->b->width];
  struct args args[g->b->width];

  for (unsigned int c = 0; c < g->b->width; c++) {
    if (g->b->type == MATRIX) {
      args[c].g = g;
      args[c].c = c;
      args[c].col_height = col_height;
      pthread_create(&threads[c], NULL, disarray_wrapper, &args[c]);
    } else {
      disarray_single_column(g, c, col_height);
    }
  }
  if (g->b->type == MATRIX) {
    for (unsigned int c = 0; c < g->b->width; c++) {
      pthread_join(threads[c], NULL);
    }
  }
    
  disarray_update_queue(g->black_queue, g->b->height, col_height);
  disarray_update_queue(g->white_queue, g->b->height, col_height);

  g->player = (g->player + 1) % 2;
  free(col_height);
}

/* Updates the queues so they reflect gravity after a offset move is made. 

   @param posqueue* the queue that we are updating
   @param pos the position of the first cell that was removed
   @param pos the position o fthe second cell that was removed. 
   */
void offset_update_queue(posqueue* q, pos c1, pos c2) {
  pq_entry* current = q->head;
  while (current) {
    pos p = current->p;
    if (p.c == c1.c && p.r < c1.r) {
      current->p.r++;
    }
    if (p.c == c2.c && p.r < c2.r) {
      current->p.r++;
    }
    current = current->next;
  }
}


bool offset(game* g) {
  if (g->white_queue->len == 0 || g->black_queue->len == 0) {
    return false;
  }

  pos c1, c2; // cell 1, cell 2
  switch (g->player) {
    case WHITES_TURN: 
      c1 = pos_dequeue(g->white_queue);
      c2 = posqueue_remback(g->black_queue);
      break;
    case BLACKS_TURN:
      c1 = pos_dequeue(g->black_queue);
      c2 = posqueue_remback(g->white_queue);
      break;
  }  

  board_set(g->b, c1, EMPTY);
  board_set(g->b, c2, EMPTY);
  
  
  for (unsigned int r = c1.r; r > 0; r--) {
    cell above = board_get(g->b, make_pos(r-1, c1.c));
    board_set(g->b, make_pos(r, c1.c), above);
  }
  board_set(g->b, make_pos(0, c1.c), EMPTY);
  
  for (unsigned int r = c2.r; r > 0; r--) {
    cell above = board_get(g->b, make_pos(r-1, c2.c));
    board_set(g->b, make_pos(r, c2.c), above);
  }
  board_set(g->b, make_pos(0, c2.c), EMPTY);
  
  offset_update_queue(g->white_queue, c1, c2);
  offset_update_queue(g->black_queue, c1, c2);

  g->player = (g->player + 1) % 2;
  return true;
}

/* Checks if a run is possible in a game starting at a certain row and column
   index and a given direction. The directions are either vertial, horizontal, 
   diagonal down right, and diagonal down left. 

   @param game* the game that we are considering
   @param unsigned int the start row index
   @param unsigned int the start column index
   @param int the direction we are looking for in the horizontal direction
   @param int the direction we are looking for in the vertical direction, 
   @param cell the player that the initial piece belongs to, also the player
   that we are checking if they made a run
   @return bool true if a run is completed, false if a run is not
   */
bool check_run(game* g, unsigned int start_r, unsigned int start_c, int dir_r,
                  int dir_c, cell player) {
  unsigned int count = 0;

  for (unsigned int i = 0; i < g->run; i++) {
    unsigned int r = start_r + i * dir_r;
    unsigned int c = start_c + i * dir_c;

    if (r >= g->b->height || c >= g->b->width) {
      return false; // Out of bounds
    }

    if (board_get(g->b, make_pos(r, c)) == player) {
      count++;
    } else {
      break;
    }
  }
  return count == g->run;
}

outcome game_outcome(game* g){
  bool white_runs = false, black_runs = false, none_empty = true;
  for (unsigned int r = 0; r < g->b->height; r++) {
    for (unsigned int c = 0; c < g->b->width; c++) {
      cell cur_c = board_get(g->b, make_pos(r,c));
      if (cur_c == EMPTY) {
        none_empty = false;
        continue;
      }

      bool made_run = check_run(g, r, c, 1, 1, cur_c);
      made_run = made_run || check_run(g, r, c, 0, 1, cur_c);
      made_run = made_run || check_run(g, r, c, 1, 0, cur_c);
      made_run = made_run || check_run(g, r, c, 1, -1, cur_c);
      if (made_run){
        switch (cur_c) {
          case BLACK:
            black_runs = true;
            break;
          case WHITE:
            white_runs = true;
        }
      }  
    }
  }
  if (black_runs && white_runs) {
    return DRAW;
  } else if (black_runs) {
    return BLACK_WIN;
  } else if (white_runs) {
    return WHITE_WIN;
  } else if (none_empty) {
    return DRAW;
  }
  return IN_PROGRESS;
}
