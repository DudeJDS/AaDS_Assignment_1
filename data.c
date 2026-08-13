#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

/*----------- Function definitions -----------*/
// Create node of wildlife encounter in linked list
wildlife_t *create_wildlife(char *fields[]) {
    wildlife_t *node = malloc(sizeof(wildlife_t));
    if (node == NULL) {
        fprintf(stderr, "Error: Malloc on wildlife node creation failed\n");
        exit(1);
    }

    int field_num = 0;
    node->key = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(node->key, fields[field_num]);
    field_num++;

    node->taxa = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(node->taxa, fields[field_num]);
    field_num++;

    node->kingdom = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(node->kingdom, fields[field_num]);
    field_num++;

    node->phylum = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(node->phylum, fields[field_num]);
    field_num++;

    node->class_name = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(node->class_name, fields[field_num]);
    field_num++;

    node->order = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(node->order, fields[field_num]);
    field_num++;

    node->family = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(node->family, fields[field_num]);
    field_num++;

    node->genus = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(node->genus, fields[field_num]);
    field_num++;

    node->species = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(node->species, fields[field_num]);
    field_num++;

    node->common_name = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(node->common_name, fields[field_num]);
    field_num++;

    node->identification = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(node->identification, fields[field_num]);
    field_num++;

    node->data_resource = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(node->data_resource, fields[field_num]);
    field_num++;

    node->sighting_data = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(node->sighting_data, fields[field_num]);
    field_num++;

    node->latitude = atof(fields[field_num]);
    field_num++;
    
    node->longitude = atof(fields[field_num]);
    field_num++;

    node->easy_adress = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(node->easy_adress, fields[field_num]);


    node->next = NULL;

    return node;
}

// Free wildlife node
void free_wildlife(wildlife_t){

}

// Helper function to allocate appropriate memory for each field
char *dup_str(const char *s) {
    if (s == NULL) {
        return NULL;
    }

    int len = strlen(s);
    if (len > MAX_FIELD_LEN) {
        fprintf(stderr, "Error: Input field of len greater than that of spec def\n");
    }

    char *dup_str = malloc(len + 1); // Account for '\0'
    if (dup_str == NULL) {
        fprintf(stderr, "Error: malloc failed\n");
    }

    strncpy(dup_str, s, len);
    dup_str[len] = '\0';
    
    return dup_str;
}