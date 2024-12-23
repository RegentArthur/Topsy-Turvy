#ifndef POS_H
#define POS_H

struct pos {
    unsigned int r, c;
};

typedef struct pos pos;


typedef struct pq_entry pq_entry;

struct pq_entry {
    pos p;
    pq_entry *next, *prev;
};


struct posqueue {
    pq_entry *head, *tail;
    unsigned int len;
};

typedef struct posqueue posqueue;

/* Creates a pos value from a given position on the game board. Note that the
   function does not check if the values inputed correspond to a valid 
   position on the board. Also note that the rows and columns are counted 
   with zero-based indexing.

   @param unsigned int the given row position
   @param unsigned int the given column position
   @return pos the pos struct corresponding to the passed position
   */
pos make_pos(unsigned int r, unsigned int c);

/* Allocates a new empty queue and returns its pointer. Note that the fields 
   of the empty struct are set to NULL, NULL, and 0 for the head pointer, 
   tail pointer, and length of list respectively. 

   @return posqueue* new, empty, queue of positions
   */
posqueue* posqueue_new();

/* Adds a position to a given position queue. Specifically, it adds a new 
   position queue entry pointer to the end of the position queue. The length
   of the position queue is increased by 1.

   @param posqueue* the position queue that we are appending a position to
   @param pos the position that we are appending
   */
void pos_enqueue(posqueue* q, pos p);

/* Removes the first element of a given position queue. It then alters the 
   given position queue to not include the removed element, and it returns
   the position struct that was removed. The length of the position queue
   is decreased by 1.

   @param posqueue* the position queue that we are removing the first element
   from 
   @return pos the position that we have removed
   */
pos pos_dequeue(posqueue* q);

/* Removes the last position of a given position queue. It alters the tail of
   the position queue directly, and it returns the position that was removed.
   The length of the position queue is also decreased by 1.

   @param posqueue* the position queue that we are removing a position from.
   @return pos the postition that was removed
   */
pos posqueue_remback(posqueue* q);

/* Entirely deallocates an existing queue.

   @param posqueue* the queue that is to be deallocated
   */
void posqueue_free(posqueue* q);

#endif /* POS_H */
