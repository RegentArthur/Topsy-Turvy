#include <stdio.h>
#include <stdlib.h>
#include "pos.h"

pos make_pos(unsigned int r, unsigned int c){
  pos res = {r, c};
  return res;
}

posqueue* posqueue_new(){
  posqueue* res = (posqueue*) malloc (sizeof(posqueue));
  if(!res) {
    fprintf(stderr, "posqueue_new, unable to allocate result\n");
    exit(1);
  }
  res->head = NULL;
  res->tail = NULL;
  res->len = 0;
  return res;
}

void posqueue_free(posqueue* q){
  if(q == NULL){
    return;
  }

  while (q->head) {
    pq_entry* temp = q->head->next;
    free(q->head);
    q->head = temp;
  }
}

void pos_enqueue(posqueue* q, pos p){
  pq_entry* entry = (pq_entry*) malloc (sizeof(pq_entry));
  if(!entry) {
    fprintf(stderr, "pos_enqueue, unable to allocate result\n");
    exit(1);
  }
  entry->p = p;
  entry->next = NULL;

  if (q->len == 0) {
    entry->prev = NULL;
    q->head = entry;
  } else {
    entry->prev = q->tail;
    q->tail->next = entry;
  }

  q->tail = entry;
  (q->len)++;
}

pos pos_dequeue(posqueue* q){
  if(q == NULL || q->len == 0 || q->head == NULL || q->tail == NULL){
    fprintf(stderr, "pos_dequeue, cannot dequeue from empty list\n");
    exit(1);
  }
  pq_entry* first_entry = q->head;
  q->head = q->head->next;
  
  if (q->head) {
    q->head->prev = NULL;
  } else {
    q->tail = NULL; // When len == 1 and removing an element empties the queue
  }

  (q->len)--;
  pos res = first_entry->p;
  free(first_entry);
  return res;
}

pos posqueue_remback(posqueue* q){
  if(q == NULL || q->len == 0 || q->head == NULL || q->tail == NULL){
    fprintf(stderr, "posqueue_remback, cannot remove from empty list\n");
    exit(1);
  }
  pq_entry* last_entry = q->tail;
  q->tail = q->tail->prev;

  if (q->tail) {
    q->tail->next = NULL;
  } else {
    q->head = NULL; // Queue is now empty
  }

  (q->len)--;
  pos res = last_entry->p;
  free(last_entry);
  return res;
}
