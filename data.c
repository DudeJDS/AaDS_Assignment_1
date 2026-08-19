#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

/*----------- Data definitions -----------*/
typedef struct wildlife {
    char *key;
    char *taxa;
    char *kingdom;
    char *phylum;
    char *class_name;
    char *order;
    char *family;
    char *genus;
    char *species;
    char *common_name;
    char *identification;
    char *data_resource;
    char *sighting_data;
    long double latitude;
    long double longitude;
    char *easy_adress;
};

/*----------- Function definitions -----------*/
// Create data of wildlife 
wildlife_t *create_wildlife(char *fields[]) {
    wildlife_t *data = malloc(sizeof(wildlife_t));
    if (data == NULL) {
        fprintf(stderr, "Error: Malloc on wildlife data creation failed\n");
        exit(1);
    }

    int field_num = 0;
    data->key = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(data->key, fields[field_num]);
    field_num++;

    data->taxa = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(data->taxa, fields[field_num]);
    field_num++;

    data->kingdom = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(data->kingdom, fields[field_num]);
    field_num++;

    data->phylum = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(data->phylum, fields[field_num]);
    field_num++;

    data->class_name = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(data->class_name, fields[field_num]);
    field_num++;

    data->order = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(data->order, fields[field_num]);
    field_num++;

    data->family = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(data->family, fields[field_num]);
    field_num++;

    data->genus = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(data->genus, fields[field_num]);
    field_num++;

    data->species = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(data->species, fields[field_num]);
    field_num++;

    data->common_name = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(data->common_name, fields[field_num]);
    field_num++;

    data->identification = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(data->identification, fields[field_num]);
    field_num++;

    data->data_resource = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(data->data_resource, fields[field_num]);
    field_num++;

    data->sighting_data = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(data->sighting_data, fields[field_num]);
    field_num++;

    data->latitude = atof(fields[field_num]);
    field_num++;
    
    data->longitude = atof(fields[field_num]);
    field_num++;

    data->easy_adress = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(data->easy_adress, fields[field_num]);

    return data;
}

// Free wildlife data
void free_wildlife(wildlife_t){
    
}


//free data, i.e. free fields first
// free data after

// data n string cmps are just how many we have done
// bit by bit cmp btw not byte

// linked list .c and .h
// dictionary .c and .h

// Get a "hello world flag" from each library

// typedef of data seperate to..
// typedef of linked list

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