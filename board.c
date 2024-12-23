#include <stdlib.h>
#include <stdio.h>
#include "board.h"

/* Raises an error if the passed type is not a supported configuration.

   @param enum type the type that we are checking
   @param char* the function name that called the check, used for error 
   logging
   */
void check_configuration(enum type type, char* function_name) {
  if (type != MATRIX && type != BITS) {
    fprintf(stderr, "%s, representation is not supported\n", function_name);
    exit(1);
  }
}

board* board_new(unsigned int width, unsigned int height, enum type type) {
  check_configuration(type, "board_new");

  board* res = (board*) malloc (sizeof(board));
  if (!res) {
    fprintf(stderr, "board_new, unable to allocate result\n");
    exit(1);
  }
  
  res->height = height;
  res->width = width;
  res->type = type;
  if (type == MATRIX) {
    res->u.matrix = (cell**) malloc (sizeof (cell*) * height);
    if (!res->u.matrix) {
      fprintf(stderr, "board_new, unable to allocate result\n");
      exit(1);
    }
    for (unsigned int i = 0; i < height; i++) {
      res->u.matrix[i]  = (cell*) malloc (sizeof (cell) * width);
      if (!res->u.matrix[i]) {
        fprintf(stderr, "board_new, unable to allocate result\n");
        exit(1);
      }
      for (unsigned int j = 0; j < width; j++) {
        res->u.matrix[i][j] = EMPTY;
      }
    }
  } else if (type == BITS) {
    unsigned int total_bits = width * height * 2;
    unsigned int reslen = (total_bits + 31) / 32;
    res->u.bits = (unsigned int*) malloc (sizeof(unsigned int) * reslen);
    if (!res->u.bits) {
      fprintf(stderr, "board_new, unable to allocate result\n");
      exit(1);
    }
    for(unsigned int i = 0; i < reslen; i++) {
      res->u.bits[i] = 0;
    }
  }

  return res;
}

void board_free(board* b){
  check_configuration(b->type, "board_free");
  
  if (b->type == MATRIX) {
    for (unsigned int r = 0; r < b->height; r++) {
      free(b->u.matrix[r]);
    }
    free(b->u.matrix);
  } else if (b->type == BITS) {
    free(b->u.bits);
  }

  free(b);
}

/* Determines the label that will be displayed for either a row or column.
   The lables follow the specifications stated in the description of the 
   board_show function: the digits 0-9, the capital letters A-Z, the lower
   case letters a-z, then '?'.

   @param unsinged int the row or column that we are finding the label for
   */
char find_label(unsigned int l) {
  if (l < 10) {
    return '0' + l;
  } 
  if (l < 36) {
    return 'A' + l - 10;
  } 
  if (l < 62) {
    return 'a' + l - 26 - 10;
  }
  return '?';
}

void board_show(board* b){
  check_configuration(b->type, "board_show");

  printf("  ");
  for (unsigned int c = 0; c < b->width; c++) {
    printf("%c", find_label(c));
  }
  printf("\n");

  for (unsigned int r = 0; r < b->height; r++) {
    printf("%c ", find_label(r));
    for(unsigned int c = 0; c < b->width; c++) {
      switch (board_get(b, make_pos(r, c))) {
        case EMPTY:
          printf(".");
          break;
        case BLACK:
          printf("*");
          break;
        case WHITE:
          printf("o");
          break;
      }
    }
    printf("\n");
  }
}


cell board_get(board* b, pos p) {
  check_configuration(b->type, "board_get");
  if ((p.r >= b->height) || (p.c >= b->width)) {
    fprintf(stderr, "board_get, position is not within the boundary of the"
        "board\n");
    exit(1);
  } 
  if (b->type == MATRIX) {
    return b->u.matrix[p.r][p.c];
  } else if (b->type == BITS) {
    unsigned int bit_index = (p.r * b->width + p.c) * 2;
    unsigned int arr_index = bit_index / 32;
    return (cell) ((b->u.bits[arr_index] >> (bit_index % 32)) & 0x3);
  }  
  return -1; //nonsense, just to return someting, but should never be reached
}


void board_set(board* b, pos p, cell c) {
  check_configuration(b->type, "board_set");
  if (p.r >= b->height || p.c >= b->width) {
    fprintf(stderr, "board_set, position is not within the board boundary\n");
    exit(1);
  }
  if (b->type == MATRIX) {
    b->u.matrix[p.r][p.c] = c;
  } else if (b->type == BITS) {
    unsigned int bit_index = (p.r * b->width + p.c) * 2;
    unsigned int arr_index = bit_index / 32;
    unsigned int offset = bit_index % 32;

    //clear previous value
    b->u.bits[arr_index] &= ~(0x3 << offset);
    //write new value
    b->u.bits[arr_index] |= ((unsigned int) c << offset);
  }
}
