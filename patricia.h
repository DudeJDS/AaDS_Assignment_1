#ifndef PATRICIA_H
#define PATRICIA_H

#include "data.h"
#include "bit.h"
#include "parse.h"

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

typedef struct radix_node radix_node_t;
struct radix_node {
    char *stem;          
    unsigned int stem_bits;
    entry_list_t *entries;
    radix_node_t *branchA;
    radix_node_t *branchB;
};

typedef struct radix_tree radix_tree_t;
struct radix_tree {
    radix_node_t *root;
    int size;
};

typedef radix_tree_t dict_t;

/*----------- Function definitions -----------*/
wildlife_entry_t *create_wildlife_entry(wildlife_t *data, wildlife_entry_t *next);
void append_wildlife_entry(entry_list_t *list, wildlife_t *data);
void free_wildlife_entries(wildlife_entry_t *entry);

char *createStem(char *oldKey, unsigned int startBit, unsigned int numBits);

radix_node_t *create_radix_node(char *stem, unsigned int stem_bits, entry_list_t *entries, radix_node_t *branchA, radix_node_t *branchB);
radix_tree_t *create_radix_tree(void);

unsigned int keyTotalBits(char *key);
entry_list_t *create_entry_list(wildlife_t *data);
char *extractSubStem(char *sourceBits, unsigned int startBit, unsigned int numBits);
radix_node_t *radix_insert(radix_node_t *node, wildlife_t *data, unsigned int bits_matched_so_far);
dict_t *dict_build(parsed_records_t *parsed_records);

radix_node_t *radix_locate(radix_node_t *root, char *query, int *exact_match, int *node_cmps, int *bit_cmps);
void collect_entry_nodes(radix_node_t *node, radix_node_t ***leaves, int *count, int *capacity, int *node_cmps);
void radix_search_by_key(radix_node_t *root, char *query, int *bit_cmps, int *str_cmps, int *node_cmps, int *records_found, FILE *outFile);

void free_radix_node(radix_node_t *node);
void free_radix_tree(radix_tree_t *tree);

int min3(int a, int b, int c);
int editDistance(char *str1, char *str2, int n, int m);


#endif