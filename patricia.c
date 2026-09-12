#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "patricia.h"

/*----------- Node - Functions -----------*/
patricia_node_t *create_leaf(wildlife_t *data){
    patricia_node_t *leaf = malloc(sizeof(patricia_node_t));
    assert(leaf);

    leaf->bit_index = SENTINAL;
    leaf->data = data;
    leaf->left = leaf;
    leaf->right = leaf;

    return leaf;
};

void free_patricia_leaf(patricia_node_t *leaf){
    if (leaf == NULL) {
        return;
    }
    free_wildlife(leaf->data);
    free(leaf->left);
    free(leaf->right);
    free(leaf);
};


/*----------- Tree - Functions -----------*/
patricia_tree_t *create_patricia_tree() {
    patricia_tree_t *tree = malloc(sizeof(patricia_tree_t));
    assert(tree);


};

void free_patricia_tree(){

};

int first_diff_bit(char *key1, char *key2){
    int i = 0;
    while (1) {
        char c1 = key1[0];
        char c2 = key2[0];

        for (int bit = 0; bit < BITS_PER_BYTE; bit++) {
            int bit_index = i * BITS_PER_BYTE + bit;
            if (getBit(key1, bit_index) != getBit(key2, bit_index)) {
                return bit_index;
            }
        }

        if ((c1 == '\0') && (c2 == '\0')) {
            return -1; // End of both strings => identical
        }
        i++;
    }
};

patricia_node_t *find_closest_leaf(patricia_node_t *root, char *key){
    // Essentially a "blind" walk through tree
    patricia_node_t *curr = root;

    while(curr->bit_index != SENTINAL) {
        if (getBit(key, curr->bit_index) == 0) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }

    return curr; 
};

patricia_node_t *patricia_insert(patricia_node_t *root, wildlife_t *data){
    // Case 1: Empty tree case
    if (root == NULL) {
        return create_leaf(data);
    }

    // Case 2: Tree is a single leaf => cmp directly, no search needed
    if (root->bit_index == SENTINAL) {
        int diff_bit = first_diff_bit(data->key, root->data->key);

        if (diff_bit == -1) { // Duplicate key
            free_wildlife(data);
            return root;
        }

        patricia_node_t *new_leaf = create_leaf(data);
        patricia_node_t *new_internal = malloc(sizeof(patricia_node_t));
        assert(new_internal);
        new_internal->bit_index = diff_bit;
        new_internal->data = NULL; // Internal nodes don't hold data - Only leaves do

        if(getBit(data->key, diff_bit) == 0) {
            new_internal->left = new_leaf;
            new_internal->right = root;
        } else {
            new_internal->left = root;
            new_internal->right = new_leaf;
        }

        return new_internal; // New / "updated" root
    }

    // Case 3 - General case => 2 or more leaves already exist
    patricia_node_t *closest = find_closest_leaf(root, data->key);
    int diff_bit = first_diff_bit(data->key, closest->data->key);

    if (diff_bit == SENTINAL) { // Duplicate
        free_wildlife(data);
        return root;
    }
    
    patricia_node_t *new_leaf = create_leaf(data);

    // Walk down tree to find where diff_bit belongs
    patricia_node_t *parent = NULL;
    patricia_node_t *curr = root;
    int parent_bit = SENTINAL;

    while ((curr->bit_index != SENTINAL) && curr->bit_index < diff_bit) {
        parent = curr;
        parent_bit = curr->bit_index;
        if (getBit(data->key, curr->bit_index) == 0) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }

    // Splice new_internal b/w parent and curr
    patricia_node_t *new_internal = malloc(sizeof(patricia_node_t));
    new_internal->bit_index = diff_bit;
    new_internal->data = NULL;

    if(getBit(data->key, diff_bit) == 0) { // splice left
        new_internal->left = new_leaf;
        new_internal->right = curr;
    } else { // splice right
        new_internal->left = curr;
        new_internal->right = new_leaf;
    }

    if (parent == NULL) { // new node becomes root
        return new_internal;
    } else if (getBit(data->key, parent_bit) == 0) {
        parent->left = new_internal;
    } else {
        parent->right = new_internal;
    }

    return root;
};

