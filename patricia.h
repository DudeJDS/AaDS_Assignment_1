#ifndef PATRICIA_H
#define PATRICIA_H

#include "data.h"
#include "bit.h""
/*----------- Constant definitions -----------*/
#define SENTINAL -1

/*----------- Data definitions -----------*/
typedef struct patricia_node patricia_node_t;
struct patricia_node {
    int bit_index; // SENTINAL => leaf NOT branch
    wildlife_t *data; // Only meaningful at leaves bcs. then we know the full key
    patricia_node_t *left; // subtree w/h bit 0
    patricia_node_t *right; // subtree w/h bit 1
};

typedef patricia_tree_t dict_t;
typedef struct patricia_tree patricia_tree_t;
struct patricia_tree {
    patricia_node_t *root;
    int size;
};

typedef patricia_tree_t dict_t;

#endif