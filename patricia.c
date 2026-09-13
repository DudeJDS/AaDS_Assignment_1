#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "patricia.h"

/*----------- Entry - Functions -----------*/
/* Basic overview of logic:
    wild_life_entry functions as a linked list for duplicate keys 
    Note: 
        Utilised only on leaf radix_nodes 
*/
wildlife_entry_t *create_wildlife_entry(wildlife_t *data, wildlife_entry_t *next) {
    wildlife_entry_t *entry = malloc(sizeof(wildlife_entry_t));
    assert(entry);
    entry->data = data;
    entry->next = next;
    return entry;
}

void append_wildlife_entry(entry_list_t *list, wildlife_t *data) {
    wildlife_entry_t *entry = create_wildlife_entry(data, NULL);
    list->tail->next = entry;
    list->tail = entry;
}

void free_wildlife_entries(wildlife_entry_t *entry) {
    while (entry != NULL) {
        wildlife_entry_t *next = entry->next;
        free_wildlife(entry->data);
        free(entry);
        entry = next;
    }
}

/*----------- Stem creation - Function -----------*/
/* Basic overview of logic:
    Take a key (str) 7 a bit range within that key [startBit, startBit + numBits] and copy only those "in range" bits into a byte array
    => instead of storing entire string, node only stores the specified splic of bits it needs
*/
char *createStem(char *oldKey, unsigned int startBit, unsigned int numBits) {
    assert(oldKey);
    int extraBytes = 0;
    if ((numBits % BITS_PER_BYTE) > 0) { // numBits != integer mult. of 8 bits (or a byte) => need extra byte for leftover bits
        extraBytes = 1;
    }
    unsigned int totalBytes = (numBits / BITS_PER_BYTE) + extraBytes; // Total byte count
    char *newStem = malloc(sizeof(char) * totalBytes);
    assert(newStem);

    // Post malloc -> zero all "junk" entries for future logic use (OR / |=)
    for (unsigned int i = 0; i < totalBytes; i++) {
        newStem[i] = 0;
    }

    // Loop copies each bit into appropriate position in newStem
    for (unsigned int i = 0; i < numBits; i++) {
        unsigned int indexFromLeft = i % BITS_PER_BYTE;
        unsigned int offset = (BITS_PER_BYTE - indexFromLeft - 1) % BITS_PER_BYTE;
        unsigned int bitMaskForPosition = 1 << offset;
        unsigned int bitValueAtPosition = getBit(oldKey, startBit + i);
        unsigned int byteInNewStem = i / BITS_PER_BYTE;
        if (bitValueAtPosition == 1) {
            newStem[byteInNewStem] |= bitMaskForPosition; // |= bitwise OR (logic mentioned above)
        }
    }
    return newStem;
}

/*----------- Tree - Functions -----------*/
radix_node_t *create_radix_node(char *stem, unsigned int stem_bits, entry_list_t *entries, radix_node_t *branchA, radix_node_t *branchB) {
    radix_node_t *node = malloc(sizeof(radix_node_t));
    assert(node);
    node->stem = stem;
    node->stem_bits = stem_bits;
    node->entries = entries;
    node->branchA = branchA;
    node->branchB = branchB;
    return node;
}

radix_tree_t *create_radix_tree(void) {
    radix_tree_t *tree = malloc(sizeof(radix_tree_t));
    assert(tree);
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

/*Basic overview:
    Simply just a concise way to find how many bits in a key string
    Mostly for readability in later function
*/ 
unsigned int keyTotalBits(char *key) {
    return (unsigned int) (strlen(key) + 1) * BITS_PER_BYTE; // As a char is 8 bits / 1 byte
}
 
entry_list_t *create_entry_list(wildlife_t *data) {
    entry_list_t *list = malloc(sizeof(entry_list_t));
    assert(list);
    wildlife_entry_t *entry = create_wildlife_entry(data, NULL);
    list->head = entry;
    list->tail = entry;
    return list;
}

/* Basic overview:
    Essentially equivelant function as createStem except now we handle the case of existing stem (raw bit array)
    Difference: creatStem accepts only null-terminated str 
                here, accepts raw bit array (not a str as no '\0').
    Use case: Used in radix_insert when an existing node's stem gets split at some bit pos.
*/ 
char *extractSubStem(char *sourceBits, unsigned int startBit, unsigned int numBits) {
    if (numBits == 0) { // In case of split of differeing the final bit of prev. stem
        return NULL;
    }
    int extraBytes = 0;
    if ((numBits % BITS_PER_BYTE) > 0) {
        extraBytes = 1;
    }
    unsigned int totalBytes = (numBits / BITS_PER_BYTE) + extraBytes;
    char *newStem = malloc(sizeof(char) * totalBytes);
    assert(newStem);
 
    for (unsigned int i = 0; i < totalBytes; i++) {
        newStem[i] = 0;
    }
    for (unsigned int i = 0; i < numBits; i++) {
        unsigned int indexFromLeft = i % BITS_PER_BYTE;
        unsigned int offset = (BITS_PER_BYTE - indexFromLeft - 1) % BITS_PER_BYTE;
        unsigned int bitMaskForPosition = 1 << offset;
        unsigned int bitValueAtPosition = getRawBit(sourceBits, startBit + i);
        unsigned int byteInNewStem = i / BITS_PER_BYTE;
        newStem[byteInNewStem] |= bitMaskForPosition * bitValueAtPosition;
    }
    return newStem;
}

/* Basic overview:
    Return either
        a. new_node (base case)
        b. returns same node called w/h only change being a branch ptr update
        c. returns same node w/h only change being updated entries list
        d. returns same node w/h reconstructed struct (actual "thing") being updated 
    bits_matched_so_far tracks how many bits of data's key have already been confirmed to mathc smth. on path down from root
*/
radix_node_t *radix_insert(radix_node_t *node, wildlife_t *data, unsigned int bits_matched_so_far) {
    unsigned int keyRemainingBits = keyTotalBits(data->key) - bits_matched_so_far; // How many accounted for bits
 
    // Base case: All remaining unmatched bits of data's key form a new node w/h the data
    if (node == NULL) {
        char *stem = createStem(data->key, bits_matched_so_far, keyRemainingBits);
        return create_radix_node(stem, keyRemainingBits, create_entry_list(data), NULL, NULL);
    }
 
    // Compare the key against this node's stem, bit by bit
    unsigned int common_len;
    if (node->stem_bits < keyRemainingBits) {
        common_len = node->stem_bits;
    } else {
        common_len = keyRemainingBits;
    }

    unsigned int mismatch_at = common_len; // track of indx where mismatch occurs
    for (unsigned int i = 0; i < common_len; i++) {
        int stem_bit = getRawBit(node->stem, i);
        int key_bit = getBit(data->key, bits_matched_so_far + i);
        if (stem_bit != key_bit) {
            mismatch_at = i;
            break;
        }
    }
 
    if (mismatch_at == common_len) {
        // Every compared bit is a match
        if (node->stem_bits == keyRemainingBits) {
            // key terminates
            if (node->entries == NULL) { // No other dup keys here
                node->entries = create_entry_list(data);
            } else {
                append_wildlife_entry(node->entries, data); // Dup keys exist => add to LL 
            }
            return node;
        } else if (node->stem_bits < keyRemainingBits) {
            // Stem exhausted fitst => whole stem matched but, key is larger => descend
            unsigned int next_bit_pos = bits_matched_so_far + node->stem_bits;
            int next_bit = getBit(data->key, next_bit_pos);
            unsigned int new_bits_matched = next_bit_pos + 1;
            if (next_bit == 0) {
                node->branchA = radix_insert(node->branchA, data, new_bits_matched);
            } else {
                node->branchB = radix_insert(node->branchB, data, new_bits_matched);
            }
            return node;
        } else {
            //Key exhausted first => impossible to branch => split node's stem at point => relocate o.g. node data and children
            unsigned int j = keyRemainingBits;
            int bit_old = getRawBit(node->stem, j);
 
            char *old_remainder_stem = extractSubStem(node->stem, j + 1, node->stem_bits - j - 1);
            radix_node_t *old_remainder = create_radix_node(
                old_remainder_stem, node->stem_bits - j - 1,
                node->entries, node->branchA, node->branchB);
 
            char *common_stem = extractSubStem(node->stem, 0, j);
            free(node->stem);
            node->stem = common_stem;
            node->stem_bits = j;
            node->entries = create_entry_list(data);
            if (bit_old == 0) {
                node->branchA = old_remainder;
                node->branchB = NULL;
            } else {
                node->branchA = NULL;
                node->branchB = old_remainder;
            }
            return node;
        }
    } else {
        // bit mismatch at index mismatch_at => split w/h two new children
        unsigned int j = mismatch_at;
        int bit_old = getRawBit(node->stem, j);
        int bit_new = getBit(data->key, bits_matched_so_far + j);
        assert(bit_old != bit_new);
 
        char *old_remainder_stem = extractSubStem(node->stem, j + 1, node->stem_bits - j - 1);
        radix_node_t *old_remainder = create_radix_node(
            old_remainder_stem, node->stem_bits - j - 1,
            node->entries, node->branchA, node->branchB);
 
        unsigned int new_remaining = keyRemainingBits - j - 1;
        char *new_leaf_stem = createStem(data->key, bits_matched_so_far + j + 1, new_remaining);
        radix_node_t *new_leaf = create_radix_node(new_leaf_stem, new_remaining, create_entry_list(data), NULL, NULL);
 
        char *common_stem = extractSubStem(node->stem, 0, j);
        free(node->stem);
        node->stem = common_stem;
        node->stem_bits = j;
        node->entries = NULL;
        if (bit_old == 0) {
            node->branchA = old_remainder;
            node->branchB = new_leaf;
        } else {
            node->branchA = new_leaf;
            node->branchB = old_remainder;
        }
        return node;
    }
}

dict_t *dict_build(parsed_records_t *parsed_records) {
    radix_tree_t *tree = create_radix_tree();
    for (int i = 0; i < parsed_records->num_records; i++) {
        tree->root = radix_insert(tree->root, parsed_records->records[i], 0);
        tree->size++;
    }
    return tree;
}

/* Basic overview:
    Given query str, walk tree as long as equality holds, return node of break point / match
*/
radix_node_t *radix_locate(radix_node_t *root, char *query, int *exact_match, int *node_cmps, int *bit_cmps) {
    *exact_match = 0;
    if (root == NULL) return NULL;
 
    radix_node_t *node = root;
    unsigned int bits_matched_so_far = 0;
 
    while (1) {
        (*node_cmps)++;
        unsigned int queryRemainingBits = keyTotalBits(query) - bits_matched_so_far;
        unsigned int common_len = (node->stem_bits < queryRemainingBits) ? node->stem_bits : queryRemainingBits;
 
        unsigned int mismatch_at = common_len;
        for (unsigned int i = 0; i < common_len; i++) {
            (*bit_cmps)++;
            int stem_bit = getRawBit(node->stem, i);
            int query_bit = getBit(query, bits_matched_so_far + i);
            if (stem_bit != query_bit) {
                mismatch_at = i;
                break;
            }
        }
 
        if (mismatch_at != common_len) {
            // divergence partway through this node's stem
            return node;
        }
 
        if (node->stem_bits == queryRemainingBits) {
            // Both the stem and the query ended together => exact match
            if (node->entries != NULL) {
                *exact_match = 1;
            }
            return node;
        }
 
        if (node->stem_bits < queryRemainingBits) {
            // Stem fully checked, query not fully exploited => check next bit
            unsigned int next_bit_pos = bits_matched_so_far + node->stem_bits;
            int next_bit = getBit(query, next_bit_pos);
            radix_node_t *next = (next_bit == 0) ? node->branchA : node->branchB;
            if (next == NULL) {
                return node; 
            }
            bits_matched_so_far = next_bit_pos + 1;
            node = next;
            // continue looping
        } else {
            // Query ran exhausted before the stem
            return node;
        }
    }
}

/* Basic overview:
    a. Post radix_locate call => now know the node of query inequality
    b. collect all daata records underneath that node as all are possible candidate matches - as per spellcheck requirement 
*/
void collect_entry_nodes(radix_node_t *node, radix_node_t ***leaves, int *count, int *capacity, int *node_cmps) {
    if (node == NULL) return;
    (*node_cmps)++;
 
    if (node->entries != NULL) {
        if (*count == *capacity) {
            *capacity *= 2;
            *leaves = realloc(*leaves, sizeof(radix_node_t *) * (*capacity));
            assert(*leaves);
        }
        (*leaves)[*count] = node;
        (*count)++;
    }
 
    collect_entry_nodes(node->branchA, leaves, count, capacity, node_cmps);
    collect_entry_nodes(node->branchB, leaves, count, capacity, node_cmps);
}

/* Notes:
    "Static void" In anticipation of Ass.3 print_entry_node is generic, thus, avoiding future mult def. errors (refinement from Ass.1)
*/
static void print_entry_node(radix_node_t *node, int *records_found, FILE *outFile) {
    for (wildlife_entry_t *e = node->entries->head; e != NULL; e = e->next) {
        (*records_found)++;
        fprintf(outFile,
            "--> KEY: %s || Taxa: %s || Kingdom: %s || Phylum: %s || Class: %s || "
            "Order: %s || Family: %s || Genus: %s || Species: %s || Common_Name: %s || "
            "Identificat: %s || Data_Resour: %s || Sighting_Da: %s || latitude: %.5Lf || "
            "longitude: %.5Lf || EZI_ADD: %s || \n",
            e->data->key,
            e->data->taxa,
            e->data->kingdom,
            e->data->phylum,
            e->data->class_name,
            e->data->order,
            e->data->family,
            e->data->genus,
            e->data->species,
            e->data->common_name,
            e->data->identification,
            e->data->data_resource,
            e->data->sighting_data,
            e->data->latitude,
            e->data->longitude,
            e->data->easy_adress);
    }
}

/* Basic overview:
    top-level func. driver2.c calls
    a. print query
    b. call radix_locate to find where query lands and if match or !match
    ci. exact match => print all records at node
    cii. non-exact match => set up dynamic array to hold possible node matches (all descendants)
        score candidates w/h edit dist against query
    d. print champion  node
*/
void radix_search_by_key(radix_node_t *root, char *query, int *bit_cmps, int *str_cmps, int *node_cmps, int *records_found, FILE *outFile) {
    fprintf(outFile, "%s\n", query);
 
    if (root == NULL) {
        fprintf(outFile, "NOTFOUND\n");
        fprintf(stdout, "%s --> 0 records found - comparisons: b0 n0 s0\n", query);
        return;
    }
 
    int exact_match = 0;
    radix_node_t *stop_node = radix_locate(root, query, &exact_match, node_cmps, bit_cmps);
    (*str_cmps)++;
 
    if (exact_match) {
        print_entry_node(stop_node, records_found, outFile);
    } else {
        int capacity = 8;
        int count = 0;
        radix_node_t **candidates = malloc(sizeof(radix_node_t *) * capacity);
        assert(candidates);
        collect_entry_nodes(stop_node, &candidates, &count, &capacity, node_cmps);
 
        int best_index = -1;
        int best_dist = -1;
        for (int i = 0; i < count; i++) {
            char *candidate_key = candidates[i]->entries->head->data->key;
            (*str_cmps)++;
            int dist = editDistance(query, candidate_key, (int) strlen(query), (int) strlen(candidate_key));
            if (best_index == -1 || dist < best_dist) {
                best_dist = dist;
                best_index = i;
            }
        }
 
        if (best_index != -1) {
            print_entry_node(candidates[best_index], records_found, outFile);
        }
        free(candidates);
    }
 
    if ((*records_found) == 0) {
        fprintf(outFile, "NOTFOUND\n");
    }
 
    fprintf(stdout, "%s --> %d records found - comparisons: b%d n%d s%d\n",
        query, *records_found, *bit_cmps, *node_cmps, *str_cmps);
}

void free_radix_node(radix_node_t *node) {
    if (node == NULL) return;
 
    free_radix_node(node->branchA);
    free_radix_node(node->branchB);
 
    if (node->entries != NULL) {
        free_wildlife_entries(node->entries->head);
        free(node->entries);
    }
    free(node->stem);
    free(node);
}

void free_radix_tree(radix_tree_t *tree) {
    if (tree == NULL) return;
    free_radix_node(tree->root);
    free(tree);
}

/*----------- Distance Functions -----------*/
int min3(int a, int b, int c) {
    if (a < b) {
        if (a < c) return a;
        return c;
    } else {
        if (b < c) return b;
        return c;
    }
}

int editDistance(char *str1, char *str2, int n, int m){
    assert(m >= 0 && n >= 0 && (str1 || m == 0) && (str2 || n == 0));
    int dp[n + 1][m + 1];
 
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            if (i == 0) {
                dp[i][j] = j;
            } else if (j == 0) {
                dp[i][j] = i;
            } else if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = min3(1 + dp[i - 1][j], 1 + dp[i][j - 1], dp[i - 1][j - 1]);
            } else {
                dp[i][j] = 1 + min3(dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]);
            }
        }
    }
    return dp[n][m];
}