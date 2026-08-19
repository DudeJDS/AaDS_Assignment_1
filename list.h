#ifndef LIST_H
#define LIST_H

#include "data.h"

/*----------- Data definitions -----------*/
typedef struct node node_t;
struct node {
    wildlife_t *data; //Points to data elem of node
    node_t *next; //Points to  next node in linked list
};

typedef struct list list_t; 
typedef struct list {
    node_t *head;
    node_t *tail;
    int size;
};

/*----------- Function definitions -----------*/
node_t *create_node(wildlife_t *data);

void free_node(node_t node);

list_t *create_list(node_t *head, node_t *tail, int size);

#endif