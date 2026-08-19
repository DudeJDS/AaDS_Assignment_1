#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "data.h"

/*----------- Data definitions -----------*/
struct node {
    wildlife_t *data; //Points to data elem of node
    node_t *next; //Points to  next node in linked list
};

typedef struct list {
    node_t *head;
    node_t *tail;
    int size;
};

/*----------- Function definitions -----------*/
void free_node(node_t node);
list_t *create_list(node_t *head, node_t *tail, int size);

/*----------- Node - Functions -----------*/
node_t *create_node(wildlife_t *data){
    node_t *node = malloc(sizeof(node_t));
    if (node == NULL) {
        fprintf(stderr, "Error: Malloc on node creation failed\n");
        exit(1);
    }

    node->data = data;
    node->next = NULL;
    
    return node;
};

void free_node(node_t node){
    
};

/*----------- List - definitions -----------*/
list_t *create_list(node_t *head, node_t *tail, int size){
    list_t *list = malloc(sizeof(list_t));
    if (list == NULL) {
        fprintf(stderr, "Error: Malloc on list creation failed\n");
        exit(1);
    }

    list->head = head;
    list->tail = tail;
    list->size = size;

    return list;
};