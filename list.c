#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "data.h"
#include "bit.h"

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
}

void free_node(node_t *node){
    if (node == NULL) {
        return;
    }

    free_wildlife(node->data);
    free(node);
}

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
}

void free_list(list_t *list){
    if (list == NULL) {
        return;
    }

    node_t *curr_node = list->head;

    while (curr_node != NULL) {
        node_t *next_node = curr_node->next;
        free_node(curr_node);
        curr_node = next_node;
    }
    
    free(list);
}

void list_search_by_key(list_t *list, char *query, int *bit_cmps, int *str_cmps, int *node_cmps, int *records_found, FILE *outFile) {
    node_t *curr_node = list->head; // Begin traversal at the head

    fprintf(outFile, "%s\n", query); // Generic print line

    while (curr_node != NULL) {
        (*str_cmps)++;
        (*node_cmps)++;
        if (key_match(query, (curr_node->data->key), bit_cmps)) {
            (*records_found)++;
            fprintf(outFile,
                "--> KEY: %s || Taxa: %s || Kingdom: %s || Phylum: %s || Class: %s || "
                "Order: %s || Family: %s || Genus: %s || Species: %s || Common_Name: %s || "
                "Identifcat: %s || Data_Resour: %s || Sighting_Da: %s || latitude: %.5Lf || "
                "longitude: %.5Lf || EZI_ADD: %s || \n",
                curr_node->data->key,
                curr_node->data->taxa,
                curr_node->data->kingdom,
                curr_node->data->phylum,
                curr_node->data->class_name,
                curr_node->data->order,
                curr_node->data->family,
                curr_node->data->genus,
                curr_node->data->species,
                curr_node->data->common_name,
                curr_node->data->identification,
                curr_node->data->data_resource,
                curr_node->data->sighting_data,
                curr_node->data->latitude,
                curr_node->data->longitude,
                curr_node->data->easy_adress);
        }

        curr_node = curr_node->next; // Advance to next node
    }

    if ((*records_found) == 0) {
        fprintf(outFile, "NOTFOUND\n");
    }

    fprintf(stdout, "%s --> %d records found - comparisons: b%d n%d s%d\n",
        query, *records_found, *bit_cmps,
        *node_cmps, *str_cmps);
}

// Return 0 for mismatch - Return 1 for match
int key_match(char *query, char *key, int *bit_cmps){
    int i = 0;
    while (1) {
        char query_char = query[i];
        char key_char = key[i];

        for (int bit = 0; bit < BITS_PER_BYTE; bit++) { // Looks byte by byte (i.e. char per char)
            (*bit_cmps)++;
            if (getBit(query, i * BITS_PER_BYTE + bit) != getBit(key, i * BITS_PER_BYTE + bit)) {
                return 0;
            }
        }

        if ((query_char == '\0') && (key_char == '\0')) {
            return 1; // End of string reached and all bits equal
        }

        i++; // Move to next byte
    }
}