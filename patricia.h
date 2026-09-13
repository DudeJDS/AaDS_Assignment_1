#ifndef PATRICIA_H
#define PATRICIA_H

#include "data.h"
#include "bit.h"
#include "parse.h"

/*----------- Constant definitions -----------*/
#define SENTINAL -1

/*----------- Data definitions -----------*/
typedef struct wildlife_entry wildlife_entry_t;
struct wildlife_entry {
    wildlife_t *data;
    wildlife_entry_t *next;
};

typedef struct entry_list entry_list_t;
struct entry_list {
    wildlife_entry_t *head;
    wildlife_entry_t *tail;
};

typedef struct patricia_node patricia_node_t;
struct patricia_node {
    int bit_index; // SENTINAL => leaf NOT branch
    entry_list_t *entries; // list of records sharing this key; only meaningful at leaves
    patricia_node_t *left; // subtree w/h bit 0
    patricia_node_t *right; // subtree w/h bit 1
};

typedef struct patricia_tree patricia_tree_t;
struct patricia_tree {
    patricia_node_t *root;
    int size;
};

typedef patricia_tree_t dict_t;

/*----------- Function definitions -----------*/
wildlife_entry_t *create_wildlife_entry(wildlife_t *data, wildlife_entry_t *next);
void append_wildlife_entry(entry_list_t *list, wildlife_t *data);
void free_wildlife_entries(wildlife_entry_t *entry);

patricia_node_t *create_leaf(wildlife_t *data);
void free_patricia_leaf(patricia_node_t *leaf);

patricia_tree_t *create_patricia_tree();

void free_patricia_tree_helper(patricia_node_t *node, int parent_bit_index);

void free_patricia_tree(patricia_tree_t *tree);

int first_diff_bit(char *key1, char *key2);

patricia_node_t *find_closest_leaf(patricia_node_t *root, char *key);

patricia_node_t *patricia_insert(patricia_node_t *root, wildlife_t *data);

dict_t *dict_build(parsed_records_t *parsed_records);

void patricia_search_by_key(patricia_node_t *root, char *query, int *bit_cmps, int *str_cmps, int *node_cmps, int *records_found, FILE *outFile);

#endif