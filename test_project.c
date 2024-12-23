#include <criterion/criterion.h>
#include <stdlib.h>
#include <stdio.h>
#include "pos.h"
#include "board.h"
#include "logic.h"

// pos.c tests
Test(posqueue_new, create_queue) {
  posqueue* q = posqueue_new();
  cr_assert_not_null(q);
  cr_assert_null(q->head);
  cr_assert_null(q->tail);
  cr_assert_eq(q->len, 0);
  posqueue_free(q);
}

Test(pos_enqueue, single_element) {
  posqueue* q = posqueue_new();
  pos_enqueue(q, make_pos(0,0));
  cr_assert_eq(q->head, q->tail);
  cr_assert_eq(q->len, 1);
  cr_assert_eq(q->head->p.r, 0);
  cr_assert_eq(q->head->p.c, 0);
  posqueue_free(q);
}

Test(pos_enqueue, multiple_elements) {
  posqueue* q = posqueue_new();
  pos_enqueue(q, make_pos(1,7));
  pos_enqueue(q, make_pos(9,20));
  cr_assert_eq(q->len, 2);
  cr_assert_eq(q->head->p.r, 1);
  cr_assert_eq(q->head->p.c, 7);
  cr_assert_eq(q->tail->p.r, 9);
  cr_assert_eq(q->tail->p.c, 20);
  posqueue_free(q);
}

Test(pos_dequeue, length_one) {
  posqueue* q = posqueue_new();
  pos p = make_pos(0, 0);
  pos_enqueue(q, p);
  pos result = pos_dequeue(q);
  cr_assert_eq(result.r, 0);
  cr_assert_eq(result.c, 0);
  cr_assert_eq(q->len, 0);
  cr_assert_null(q->head);
  cr_assert_null(q->tail);
  posqueue_free(q);
}

Test(pos_dequeue, length_multiple) {
  posqueue* q = posqueue_new();
  pos p1 = make_pos(1, 1);
  pos p2 = make_pos(2, 2);
  pos_enqueue(q, p1);
  pos_enqueue(q, p2);
  pos result = pos_dequeue(q);
  cr_assert_eq(result.r, 1);
  cr_assert_eq(result.c, 1);
  cr_assert_eq(q->len, 1);
  cr_assert_eq(q->head->p.r, 2);
  cr_assert_eq(q->head->p.c, 2);
  posqueue_free(q);
}

Test(posqueue_remback, length_one) {
  posqueue* q = posqueue_new();
  pos p = make_pos(0, 0);
  pos_enqueue(q, p);
  pos result = posqueue_remback(q);
  cr_assert_eq(result.r, 0);
  cr_assert_eq(result.c, 0);
  cr_assert_eq(q->len, 0);
  cr_assert_null(q->head);
  cr_assert_null(q->tail);
  posqueue_free(q);
}

Test(posqueue_remback, length_multiple) {
  posqueue* q = posqueue_new();
  pos p1 = make_pos(1, 1);
  pos p2 = make_pos(2, 2);
  pos_enqueue(q, p1);
  pos_enqueue(q, p2);
  pos result = posqueue_remback(q);
  cr_assert_eq(result.r, 2);
  cr_assert_eq(result.c, 2);
  cr_assert_eq(q->len, 1);
  cr_assert_eq(q->tail->p.r, 1);
  cr_assert_eq(q->tail->p.c, 1);
  posqueue_free(q);
}

Test(posqueue_free, empty_queue) {
  posqueue* q = posqueue_new();
  posqueue_free(q);
}

Test(posqueue_free, non_empty_queue) {
  posqueue* q = posqueue_new();
  pos_enqueue(q, make_pos(0, 0));
  pos_enqueue(q, make_pos(1, 1));
  posqueue_free(q);
}

// board.c tests
Test(board_new, small_board) {
  board* b = board_new(5, 5, MATRIX);
  cr_assert_not_null(b);
  cr_assert_eq(b->width, 5);
  cr_assert_eq(b->height, 5);
  cr_assert_eq(b->type, MATRIX);

  for (unsigned int r = 0; r < 5; r++) {
    for (unsigned int c = 0; c < 5; c++) {
      cr_assert_eq(b->u.matrix[r][c], EMPTY);
    }
  }
  board_free(b);
}

Test(board_new, large_board) {
  board* b = board_new(20000, 20000, MATRIX);
  cr_assert_not_null(b);
  cr_assert_eq(b->width, 20000);
  cr_assert_eq(b->height, 20000);
  cr_assert_eq(b->type, MATRIX);

  for (unsigned int r = 0; r < 20000; r++) {
    for (unsigned int c = 0; c < 20000; c++) {
      cr_assert_eq(b->u.matrix[r][c], EMPTY);
    }
  }
  board_free(b);
}

Test(board_free, empty_board) {
  board* b = board_new(5, 5, MATRIX);
  board_free(b);
  cr_assert(1);
}

Test(board_get, random_position) {
  board* b = board_new(7, 7, MATRIX);
  pos p = make_pos(3,6);
  cr_assert_eq(board_get(b, p), EMPTY);
  cell temp = BLACK;
  b->u.matrix[p.r][p.c] = temp; 
  cr_assert_eq(board_get(b, p), BLACK);
  board_free(b);
}

Test(board_get, corner_case) {
  board* b = board_new(5, 5, MATRIX);
  pos p = make_pos(4, 4);
  cr_assert_eq(board_get(b, p), EMPTY);
  board_free(b);
}

Test(board_set, position) {
  board* b = board_new(5, 5, MATRIX);
  pos p = make_pos(1, 1);
  board_set(b, p, BLACK);
  cr_assert_eq(board_get(b, p), BLACK);
  board_set(b, p, WHITE);
  cr_assert_eq(board_get(b, p), WHITE);
  board_free(b);
}

Test(board_set, corner_case) {
  board* b = board_new(5, 5, MATRIX);
  pos p = make_pos(4, 4);
  cr_assert_eq(board_get(b, p), EMPTY);
  board_set(b, p, BLACK);
  cr_assert_eq(board_get(b, p), BLACK);
  board_free(b);
}

Test(board_show, show_empty_board) {
  board* b = board_new(5, 5, MATRIX);
  //board_show(b);
  board_free(b);
  cr_assert(1);
}

Test(board_show, large_board) {
  board* b = board_new(78, 78, MATRIX);
  //board_show(b);
  board_free(b);
  cr_assert(1);
}

Test(board_show, board_with_pieces) {
  board* b = board_new(5, 5, MATRIX);
  board_set(b, make_pos(0, 0), BLACK);
  board_set(b, make_pos(4, 4), WHITE);
  //board_show(b); 
  board_free(b);
  cr_assert(1);
}

// tests for logic.c
Test(new_game, create_valid_game) {
  game* g = new_game(4, 5, 5, MATRIX);
  cr_assert_not_null(g);
  cr_assert_eq(g->run, 4);
  cr_assert_eq(g->b->width, 5);
  cr_assert_eq(g->b->height, 5);
  cr_assert_eq(g->player, BLACKS_TURN);
  cr_assert_eq(g->black_queue->len, 0);
  cr_assert_eq(g->white_queue->len, 0);
  game_free(g);
}

Test(new_game, only_one_run_possible) {
  game* g = new_game(10, 10, 1, MATRIX);
  cr_assert_not_null(g);
  cr_assert_eq(g->run, 10);
  cr_assert_eq(g->b->width, 10);
  cr_assert_eq(g->b->height, 1);
  cr_assert_eq(g->player, BLACKS_TURN);
  cr_assert_eq(g->black_queue->len, 0);
  cr_assert_eq(g->white_queue->len, 0);
  game_free(g);
}

Test(drop_piece, drop_into_empty_column) {
  game* g = new_game(4, 5, 5, MATRIX);
  cr_assert(drop_piece(g, 2));
  cr_assert_eq(board_get(g->b, make_pos(4, 2)), BLACK);
  cr_assert_eq(g->player, WHITES_TURN);
  cr_assert_eq(g->black_queue->len, 1);
  game_free(g);
}

Test(drop_piece, drop_into_partially_filled_column) {
  game* g = new_game(4, 5, 5, MATRIX);
  drop_piece(g, 2);
  cr_assert(drop_piece(g, 2));
  cr_assert_eq(board_get(g->b, make_pos(3, 2)), WHITE);
  cr_assert_eq(board_get(g->b, make_pos(4, 2)), BLACK);
  cr_assert_eq(g->player, BLACKS_TURN);
  cr_assert_eq(g->white_queue->len, 1);
  game_free(g);
}

Test(drop_piece, drop_into_full_column) {
  game* g = new_game(4, 5, 5, MATRIX);
  for (unsigned int i = 0; i < 5; i++) {
    cr_assert(drop_piece(g, 2));
  }
  cr_assert_not(drop_piece(g, 2)); // Column is now full
  cr_assert_eq(g->player, WHITES_TURN);
  game_free(g);
}

Test(disarray, empty_board) {
  game* g = new_game(4, 5, 5, MATRIX);
  disarray(g);
  for (unsigned int r = 0; r < g->b->height; r++) {
    for (unsigned int c = 0; c < g->b->width; c++) {
      cr_assert_eq(board_get(g->b, make_pos(r, c)), EMPTY);
    }
  }
  game_free(g);
}

Test(disarray, single_piece_at_bottom) {
  game* g = new_game(4, 5, 5, MATRIX);
  drop_piece(g, 2); // Black drops in column 2
  disarray(g);
  cr_assert_eq(board_get(g->b, make_pos(4, 2)), BLACK);
  for (unsigned int r = 0; r < g->b->height - 2; r++) {
    cr_assert_eq(board_get(g->b, make_pos(r, 2)), EMPTY);
  }
  game_free(g);
}

Test(disarray, multiple_pieces_in_column) {
  game* g = new_game(4, 5, 5, MATRIX);  
  drop_piece(g, 2);
  drop_piece(g, 2);
  drop_piece(g, 2);
  disarray(g);
  cr_assert_eq(board_get(g->b, make_pos(4, 2)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(3, 2)), WHITE);
  cr_assert_eq(board_get(g->b, make_pos(2, 2)), BLACK);
  game_free(g);
}

Test(disarray, pieces_in_multiple_columns) {
  game* g = new_game(4, 5, 5, MATRIX);
  drop_piece(g, 0);
  drop_piece(g, 1);
  drop_piece(g, 1);
  drop_piece(g, 2);
  drop_piece(g, 3);
  drop_piece(g, 3);
  disarray(g);
  cr_assert_eq(board_get(g->b, make_pos(4, 0)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(4, 1)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(3, 1)), WHITE);
  cr_assert_eq(board_get(g->b, make_pos(4, 2)), WHITE);
  cr_assert_eq(board_get(g->b, make_pos(4, 3)), WHITE);
  cr_assert_eq(board_get(g->b, make_pos(3, 3)), BLACK);
  
  cr_assert_eq(g->black_queue->head->p.c, 0);
  cr_assert_eq(g->black_queue->head->p.r, 4);
  g->black_queue->head = g->black_queue->head->next;
  
  cr_assert_eq(g->black_queue->head->p.c, 1);
  cr_assert_eq(g->black_queue->head->p.r, 4);
  g->black_queue->head = g->black_queue->head->next;

  cr_assert_eq(g->white_queue->head->p.c, 1);
  cr_assert_eq(g->white_queue->head->p.r, 3);
  g->white_queue->head = g->white_queue->head->next;

  cr_assert_eq(g->black_queue->head->p.c, 3);
  cr_assert_eq(g->black_queue->head->p.r, 3);
  g->black_queue->head = g->black_queue->head->next;

  cr_assert_eq(g->white_queue->head->p.c, 2);
  cr_assert_eq(g->white_queue->head->p.r, 4);
  g->white_queue->head = g->white_queue->head->next;

  cr_assert_eq(g->white_queue->head->p.c, 3);
  cr_assert_eq(g->white_queue->head->p.r, 4);
  g->white_queue->head = g->white_queue->head->next;

  game_free(g);
}

Test(disarray, full_column) {
  game* g = new_game(4, 5, 5, MATRIX);
  for (unsigned int i = 0; i < 5; i++) {
    drop_piece(g, 2);
  }
  disarray(g);
  for (unsigned int r = 0; r < g->b->height; r++) {
    if (r % 2 == 0) {
      cr_assert_eq(board_get(g->b, make_pos(r, 2)), BLACK);
    } else {
      cr_assert_eq(board_get(g->b, make_pos(r, 2)), WHITE);
    }
  }
  game_free(g);
}

Test(disarray, check_queues) {
  game* g = new_game(4, 3, 5, MATRIX);
  drop_piece(g, 0);
  drop_piece(g, 0);
  drop_piece(g, 1);
  drop_piece(g, 2);
  drop_piece(g, 1);
  drop_piece(g, 0);
  drop_piece(g, 0);
  drop_piece(g, 1);
  drop_piece(g, 0);
  drop_piece(g, 1);
  
  disarray(g);
  
  cr_assert_eq(board_get(g->b, make_pos(4, 0)), BLACK); 
  cr_assert_eq(board_get(g->b, make_pos(3, 0)), BLACK); 
  cr_assert_eq(board_get(g->b, make_pos(2, 0)), WHITE);
  cr_assert_eq(board_get(g->b, make_pos(1, 0)), WHITE);
  cr_assert_eq(board_get(g->b, make_pos(0, 0)), BLACK);
  
  cr_assert_eq(board_get(g->b, make_pos(4, 1)), WHITE); 
  cr_assert_eq(board_get(g->b, make_pos(3, 1)), WHITE); 
  cr_assert_eq(board_get(g->b, make_pos(2, 1)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(1, 1)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(0, 1)), EMPTY); 

  cr_assert_eq(board_get(g->b, make_pos(4, 2)), WHITE); 
  cr_assert_eq(board_get(g->b, make_pos(3, 2)), EMPTY);
  cr_assert_eq(board_get(g->b, make_pos(2, 2)), EMPTY);
  cr_assert_eq(board_get(g->b, make_pos(1, 2)), EMPTY);
  cr_assert_eq(board_get(g->b, make_pos(0, 2)), EMPTY);
  
  cr_assert_eq(g->black_queue->head->p.c, 0);
  cr_assert_eq(g->black_queue->head->p.r, 0);
  g->black_queue->head = g->black_queue->head->next;

  cr_assert_eq(g->white_queue->head->p.c, 0);
  cr_assert_eq(g->white_queue->head->p.r, 1);
  g->white_queue->head = g->white_queue->head->next;
  
  cr_assert_eq(g->black_queue->head->p.c, 1);
  cr_assert_eq(g->black_queue->head->p.r, 1);
  g->black_queue->head = g->black_queue->head->next;

  cr_assert_eq(g->white_queue->head->p.c, 2);
  cr_assert_eq(g->white_queue->head->p.r, 4);
  g->white_queue->head = g->white_queue->head->next;

  cr_assert_eq(g->black_queue->head->p.c, 1);
  cr_assert_eq(g->black_queue->head->p.r, 2);
  g->black_queue->head = g->black_queue->head->next;

  cr_assert_eq(g->white_queue->head->p.c, 0);
  cr_assert_eq(g->white_queue->head->p.r, 2);
  g->white_queue->head = g->white_queue->head->next;

  cr_assert_eq(g->black_queue->head->p.c, 0);
  cr_assert_eq(g->black_queue->head->p.r, 3);
  g->black_queue->head = g->black_queue->head->next;

  cr_assert_eq(g->white_queue->head->p.c, 1);
  cr_assert_eq(g->white_queue->head->p.r, 3);
  g->white_queue->head = g->white_queue->head->next;

  cr_assert_eq(g->black_queue->head->p.c, 0);
  cr_assert_eq(g->black_queue->head->p.r, 4);
  g->black_queue->head = g->black_queue->head->next;

  cr_assert_eq(g->white_queue->head->p.c, 1);
  cr_assert_eq(g->white_queue->head->p.r, 4);
  g->white_queue->head = g->white_queue->head->next;

  game_free(g);
}

Test(offset, empty_board) {
  game* g = new_game(4, 5, 5, MATRIX);
  cr_assert_not(offset(g));
  game_free(g);
}

Test(offset, one_piece_each_player) {
  game* g = new_game(4, 5, 5, MATRIX);
  drop_piece(g, 0); // Black drops
  drop_piece(g, 1); // White drops
  cr_assert(offset(g));
  cr_assert_eq(board_get(g->b, make_pos(4, 0)), EMPTY);
  cr_assert_eq(board_get(g->b, make_pos(4, 1)), EMPTY);
  game_free(g);
}

Test(offset, multiple_pieces_in_columns) {
  game* g = new_game(4, 5, 5, MATRIX);
  drop_piece(g, 0);
  drop_piece(g, 0);
  drop_piece(g, 1);
  drop_piece(g, 1);
  cr_assert(offset(g));
  cr_assert_eq(board_get(g->b, make_pos(4, 0)), WHITE);
  cr_assert_eq(board_get(g->b, make_pos(3, 0)), EMPTY);
  cr_assert_eq(board_get(g->b, make_pos(4, 1)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(3, 1)), EMPTY);
  game_free(g);
}

Test(offset, full_column_gravity_applied) {
  game* g = new_game(4, 5, 5, MATRIX);
  for (unsigned int i = 0; i < 5; i++) {
    drop_piece(g, 0);
    drop_piece(g, 1);
  }
  cr_assert(offset(g));
  
  for (unsigned int r = 1; r < g->b->height; r++) {
    cr_assert_eq(board_get(g->b, make_pos(r, 0)), BLACK);
    cr_assert_eq(board_get(g->b, make_pos(r, 1)), WHITE);
  }
  cr_assert_eq(board_get(g->b, make_pos(0, 0)), EMPTY);
  cr_assert_eq(board_get(g->b, make_pos(0, 1)), EMPTY);
  game_free(g);
}

Test(offset, checkerboard_configuration) {
  game* g = new_game(4, 5, 5, MATRIX);
  for (unsigned char i = 0; i < 3; i++) { 
    drop_piece(g, 0);
    drop_piece(g, 1);
    drop_piece(g, 2);
    drop_piece(g, 3);
    drop_piece(g, 4);
  }
  cr_assert(offset(g));
  
  cr_assert_eq(board_get(g->b, make_pos(4, 0)), BLACK); 
  cr_assert_eq(board_get(g->b, make_pos(4, 1)), BLACK); 
  cr_assert_eq(board_get(g->b, make_pos(4, 2)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(4, 3)), WHITE);
  cr_assert_eq(board_get(g->b, make_pos(4, 4)), BLACK);
  
  cr_assert_eq(board_get(g->b, make_pos(3, 0)), WHITE); 
  cr_assert_eq(board_get(g->b, make_pos(3, 1)), WHITE); 
  cr_assert_eq(board_get(g->b, make_pos(3, 2)), WHITE);
  cr_assert_eq(board_get(g->b, make_pos(3, 3)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(3, 4)), WHITE); 

  cr_assert_eq(board_get(g->b, make_pos(2, 0)), BLACK); 
  cr_assert_eq(board_get(g->b, make_pos(2, 1)), EMPTY);
  cr_assert_eq(board_get(g->b, make_pos(2, 2)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(2, 3)), WHITE);
  cr_assert_eq(board_get(g->b, make_pos(2, 4)), EMPTY);
  game_free(g);
}

Test(offset, both_one_col) {
  game* g = new_game(4, 3, 5, MATRIX);
  drop_piece(g, 0);
  drop_piece(g, 0);
  drop_piece(g, 1);
  drop_piece(g, 2);
  drop_piece(g, 1);
  drop_piece(g, 0);
  drop_piece(g, 0);
  drop_piece(g, 1);
  drop_piece(g, 0);
  drop_piece(g, 1);
  disarray(g);
  cr_assert(offset(g));
     
  cr_assert_eq(board_get(g->b, make_pos(4, 0)), BLACK); 
  cr_assert_eq(board_get(g->b, make_pos(3, 0)), WHITE); 
  cr_assert_eq(board_get(g->b, make_pos(2, 0)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(1, 0)), EMPTY);
  cr_assert_eq(board_get(g->b, make_pos(0, 0)), EMPTY);
  
  cr_assert_eq(board_get(g->b, make_pos(4, 1)), WHITE); 
  cr_assert_eq(board_get(g->b, make_pos(3, 1)), WHITE); 
  cr_assert_eq(board_get(g->b, make_pos(2, 1)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(1, 1)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(0, 1)), EMPTY); 

  cr_assert_eq(board_get(g->b, make_pos(4, 2)), WHITE); 
  cr_assert_eq(board_get(g->b, make_pos(3, 2)), EMPTY);
  cr_assert_eq(board_get(g->b, make_pos(2, 2)), EMPTY);
  cr_assert_eq(board_get(g->b, make_pos(1, 2)), EMPTY);
  cr_assert_eq(board_get(g->b, make_pos(0, 2)), EMPTY);
  
  cr_assert_eq(g->black_queue->head->p.c, 0);
  cr_assert_eq(g->black_queue->head->p.r, 2);
  g->black_queue->head = g->black_queue->head->next;
  
  cr_assert_eq(g->black_queue->head->p.c, 1);
  cr_assert_eq(g->black_queue->head->p.r, 1);
  g->black_queue->head = g->black_queue->head->next;

  cr_assert_eq(g->white_queue->head->p.c, 2);
  cr_assert_eq(g->white_queue->head->p.r, 4);
  g->white_queue->head = g->white_queue->head->next;

  cr_assert_eq(g->black_queue->head->p.c, 1);
  cr_assert_eq(g->black_queue->head->p.r, 2);
  g->black_queue->head = g->black_queue->head->next;

  cr_assert_eq(g->white_queue->head->p.c, 0);
  cr_assert_eq(g->white_queue->head->p.r, 3);
  g->white_queue->head = g->white_queue->head->next;

  cr_assert_eq(g->black_queue->head->p.c, 0);
  cr_assert_eq(g->black_queue->head->p.r, 4);
  g->black_queue->head = g->black_queue->head->next;

  cr_assert_eq(g->white_queue->head->p.c, 1);
  cr_assert_eq(g->white_queue->head->p.r, 3);
  g->white_queue->head = g->white_queue->head->next;

  cr_assert_eq(g->white_queue->head->p.c, 1);
  cr_assert_eq(g->white_queue->head->p.r, 4);
  g->white_queue->head = g->white_queue->head->next;

  game_free(g);
}

Test(game_outcome, in_progress) {
  game* g = new_game(4, 5, 5, MATRIX);
  drop_piece(g, 0);
  drop_piece(g, 1);
  cr_assert_eq(game_outcome(g), IN_PROGRESS);
  game_free(g);
}

Test(game_outcome, horizontal_run) {
  game* g = new_game(4, 5, 5, MATRIX);
  drop_piece(g, 0);
  drop_piece(g, 4);
  drop_piece(g, 1);
  drop_piece(g, 4);
  drop_piece(g, 2);
  drop_piece(g, 4);
  drop_piece(g, 3);
  cr_assert_eq(game_outcome(g), BLACK_WIN);
  game_free(g);
}

Test(game_outcome, vertical_run) {
  game* g = new_game(4, 5, 5, MATRIX);
  drop_piece(g, 0);
  drop_piece(g, 1);
  drop_piece(g, 0);
  drop_piece(g, 1);
  drop_piece(g, 0);
  drop_piece(g, 1);
  drop_piece(g, 2);
  drop_piece(g, 1);
  cr_assert_eq(game_outcome(g), WHITE_WIN);
  game_free(g);
}

Test(game_outcome, diagonal_run) {
  game* g = new_game(3, 3, 3, MATRIX);
  drop_piece(g, 1);
  drop_piece(g, 0);
  drop_piece(g, 2);
  drop_piece(g, 1);
  drop_piece(g, 2);
  drop_piece(g, 2);
  cr_assert_eq(game_outcome(g), WHITE_WIN);
  game_free(g);
}

Test(game_outcome, top_left_diagonal_run) {
  game* g = new_game(3, 3, 3, MATRIX);
  drop_piece(g, 2);
  drop_piece(g, 1);
  drop_piece(g, 1);
  drop_piece(g, 0);
  drop_piece(g, 2);
  drop_piece(g, 0);
  drop_piece(g, 0);
  cr_assert_eq(game_outcome(g), BLACK_WIN);
  game_free(g);
}

Test(game_outcome, draw_condition) {
  game* g = new_game(4, 5, 5, MATRIX);
  drop_piece(g, 0);
  drop_piece(g, 1);
  drop_piece(g, 0);
  drop_piece(g, 1);
  drop_piece(g, 0);
  drop_piece(g, 1);
  drop_piece(g, 0);
  drop_piece(g, 1);
  drop_piece(g, 0);
  drop_piece(g, 1);
  cr_assert_eq(game_outcome(g), DRAW);
  game_free(g);
}

Test(game_outcome, full_board_draw) {
  game* g = new_game(3, 3, 3, MATRIX);
  drop_piece(g, 1);
  drop_piece(g, 0);
  drop_piece(g, 0);
  drop_piece(g, 2);
  drop_piece(g, 2);
  drop_piece(g, 1);
  drop_piece(g, 0);
  drop_piece(g, 1);
  drop_piece(g, 2);
  cr_assert_eq(game_outcome(g), DRAW);
  game_free(g);
}


// Bits test
Test(board_new, small_board_bits) {
  board* b = board_new(5, 5, BITS);
  cr_assert_not_null(b);
  cr_assert_eq(b->width, 5);
  cr_assert_eq(b->height, 5);
  cr_assert_eq(b->type, BITS);

  board_free(b);
}

Test(board_new, large_board_bits) {
  board* b = board_new(20000, 20000, BITS);
  cr_assert_not_null(b);
  cr_assert_eq(b->width, 20000);
  cr_assert_eq(b->height, 20000);
  cr_assert_eq(b->type, BITS);

  board_free(b);
}

Test(board_free, empty_board_bits) {
  board* b = board_new(5, 5, BITS);
  board_free(b);
  cr_assert(1);
}

Test(board_get, random_position_bits) {
  board* b = board_new(7, 7, BITS);
  pos p = make_pos(3,6);
  cr_assert_eq(board_get(b, p), EMPTY);
  unsigned int bit_index = (3 * 7 + 6) * 2;
  unsigned int offset = bit_index % 32; 

  b->u.bits[1] |= (0b01 << offset);  
  cr_assert_eq(board_get(b, p), BLACK);
  board_free(b);
}

Test(board_get, corner_case_bits) {
  board* b = board_new(5, 5, BITS);
  pos p = make_pos(4, 4);
  cr_assert_eq(board_get(b, p), EMPTY);
  board_free(b);
}

Test(board_set, position_bits) {
  board* b = board_new(5, 5, BITS);
  pos p = make_pos(1, 1);
  board_set(b, p, BLACK);
  cr_assert_eq(board_get(b, p), BLACK);
  board_set(b, p, WHITE);
  cr_assert_eq(board_get(b, p), WHITE);
  board_free(b);
}

Test(board_set, corner_case_bits) {
  board* b = board_new(5, 5, BITS);
  pos p = make_pos(4, 4);
  cr_assert_eq(board_get(b, p), EMPTY);
  board_set(b, p, BLACK);
  cr_assert_eq(board_get(b, p), BLACK);
  board_free(b);
}

Test(board_show, show_empty_board_bits) {
  board* b = board_new(5, 5, BITS);
  //board_show(b);
  board_free(b);
  cr_assert(1);
}

Test(board_show, large_board_bits) {
  board* b = board_new(78, 78, BITS);
  //board_show(b);
  board_free(b);
  cr_assert(1);
}

Test(board_show, board_with_pieces_bits) {
  board* b = board_new(5, 5, BITS);
  board_set(b, make_pos(0, 0), BLACK);
  board_set(b, make_pos(4, 4), WHITE);
  //board_show(b); 
  board_free(b);
  cr_assert(1);
}

Test(disarray, pieces_in_multiple_columns_bits) {
  game* g = new_game(4, 5, 5, BITS);
  drop_piece(g, 0);
  drop_piece(g, 1);
  drop_piece(g, 1);
  drop_piece(g, 2);
  drop_piece(g, 3);
  drop_piece(g, 3);
  disarray(g);
  cr_assert_eq(board_get(g->b, make_pos(4, 0)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(4, 1)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(3, 1)), WHITE);
  cr_assert_eq(board_get(g->b, make_pos(4, 2)), WHITE);
  cr_assert_eq(board_get(g->b, make_pos(4, 3)), WHITE);
  cr_assert_eq(board_get(g->b, make_pos(3, 3)), BLACK);
  
  cr_assert_eq(g->black_queue->head->p.c, 0);
  cr_assert_eq(g->black_queue->head->p.r, 4);
  g->black_queue->head = g->black_queue->head->next;
  
  cr_assert_eq(g->black_queue->head->p.c, 1);
  cr_assert_eq(g->black_queue->head->p.r, 4);
  g->black_queue->head = g->black_queue->head->next;

  cr_assert_eq(g->white_queue->head->p.c, 1);
  cr_assert_eq(g->white_queue->head->p.r, 3);
  g->white_queue->head = g->white_queue->head->next;

  cr_assert_eq(g->black_queue->head->p.c, 3);
  cr_assert_eq(g->black_queue->head->p.r, 3);
  g->black_queue->head = g->black_queue->head->next;

  cr_assert_eq(g->white_queue->head->p.c, 2);
  cr_assert_eq(g->white_queue->head->p.r, 4);
  g->white_queue->head = g->white_queue->head->next;

  cr_assert_eq(g->white_queue->head->p.c, 3);
  cr_assert_eq(g->white_queue->head->p.r, 4);
  g->white_queue->head = g->white_queue->head->next;

  game_free(g);
}

Test(offset, checkerboard_configuration_bits) {
  game* g = new_game(4, 5, 5, BITS);
  for (unsigned char i = 0; i < 3; i++) { 
    drop_piece(g, 0);
    drop_piece(g, 1);
    drop_piece(g, 2);
    drop_piece(g, 3);
    drop_piece(g, 4);
  }
  cr_assert(offset(g));
  
  cr_assert_eq(board_get(g->b, make_pos(4, 0)), BLACK); 
  cr_assert_eq(board_get(g->b, make_pos(4, 1)), BLACK); 
  cr_assert_eq(board_get(g->b, make_pos(4, 2)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(4, 3)), WHITE);
  cr_assert_eq(board_get(g->b, make_pos(4, 4)), BLACK);
  
  cr_assert_eq(board_get(g->b, make_pos(3, 0)), WHITE); 
  cr_assert_eq(board_get(g->b, make_pos(3, 1)), WHITE); 
  cr_assert_eq(board_get(g->b, make_pos(3, 2)), WHITE);
  cr_assert_eq(board_get(g->b, make_pos(3, 3)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(3, 4)), WHITE); 

  cr_assert_eq(board_get(g->b, make_pos(2, 0)), BLACK); 
  cr_assert_eq(board_get(g->b, make_pos(2, 1)), EMPTY);
  cr_assert_eq(board_get(g->b, make_pos(2, 2)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(2, 3)), WHITE);
  cr_assert_eq(board_get(g->b, make_pos(2, 4)), EMPTY);
  game_free(g);
}

Test(offset, both_one_col_bits) {
  game* g = new_game(4, 3, 5, BITS);
  drop_piece(g, 0);
  drop_piece(g, 0);
  drop_piece(g, 1);
  drop_piece(g, 2);
  drop_piece(g, 1);
  drop_piece(g, 0);
  drop_piece(g, 0);
  drop_piece(g, 1);
  drop_piece(g, 0);
  drop_piece(g, 1);
  disarray(g);
  cr_assert(offset(g));
     
  cr_assert_eq(board_get(g->b, make_pos(4, 0)), BLACK); 
  cr_assert_eq(board_get(g->b, make_pos(3, 0)), WHITE); 
  cr_assert_eq(board_get(g->b, make_pos(2, 0)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(1, 0)), EMPTY);
  cr_assert_eq(board_get(g->b, make_pos(0, 0)), EMPTY);
  
  cr_assert_eq(board_get(g->b, make_pos(4, 1)), WHITE); 
  cr_assert_eq(board_get(g->b, make_pos(3, 1)), WHITE); 
  cr_assert_eq(board_get(g->b, make_pos(2, 1)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(1, 1)), BLACK);
  cr_assert_eq(board_get(g->b, make_pos(0, 1)), EMPTY); 

  cr_assert_eq(board_get(g->b, make_pos(4, 2)), WHITE); 
  cr_assert_eq(board_get(g->b, make_pos(3, 2)), EMPTY);
  cr_assert_eq(board_get(g->b, make_pos(2, 2)), EMPTY);
  cr_assert_eq(board_get(g->b, make_pos(1, 2)), EMPTY);
  cr_assert_eq(board_get(g->b, make_pos(0, 2)), EMPTY);
  
  cr_assert_eq(g->black_queue->head->p.c, 0);
  cr_assert_eq(g->black_queue->head->p.r, 2);
  g->black_queue->head = g->black_queue->head->next;
  
  cr_assert_eq(g->black_queue->head->p.c, 1);
  cr_assert_eq(g->black_queue->head->p.r, 1);
  g->black_queue->head = g->black_queue->head->next;

  cr_assert_eq(g->white_queue->head->p.c, 2);
  cr_assert_eq(g->white_queue->head->p.r, 4);
  g->white_queue->head = g->white_queue->head->next;

  cr_assert_eq(g->black_queue->head->p.c, 1);
  cr_assert_eq(g->black_queue->head->p.r, 2);
  g->black_queue->head = g->black_queue->head->next;

  cr_assert_eq(g->white_queue->head->p.c, 0);
  cr_assert_eq(g->white_queue->head->p.r, 3);
  g->white_queue->head = g->white_queue->head->next;

  cr_assert_eq(g->black_queue->head->p.c, 0);
  cr_assert_eq(g->black_queue->head->p.r, 4);
  g->black_queue->head = g->black_queue->head->next;

  cr_assert_eq(g->white_queue->head->p.c, 1);
  cr_assert_eq(g->white_queue->head->p.r, 3);
  g->white_queue->head = g->white_queue->head->next;

  cr_assert_eq(g->white_queue->head->p.c, 1);
  cr_assert_eq(g->white_queue->head->p.r, 4);
  g->white_queue->head = g->white_queue->head->next;

  game_free(g);
}


