#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include "data.h"
#include "parse.h"

/*----------- Data definitions -----------*/
typedef struct node node_t;
struct node {
    wildlife_t *data; //Points to data elem of node
    node_t *next; //Points to  next node in linked list
};

typedef struct list list_t; 
struct list {
    node_t *head;
    node_t *tail;
    int size;
};

typedef list_t dict_t;

/*----------- Function definitions -----------*/
node_t *create_node(wildlife_t *data);

void free_node(node_t *node);

list_t *create_list(node_t *head, node_t *tail, int size);

void free_list(list_t *list);

list_t *list_create_from_records(parsed_records_t *parsed);
dict_t *dict_build(parsed_records_t *parsed);

void list_search_by_key(list_t *list, char *query, int *bit_cmps, int *str_cmps, int *node_cmps, int *records_found, FILE *outFile);

#endif