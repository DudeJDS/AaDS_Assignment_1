#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

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

    data->latitude = strtold(fields[field_num], NULL);
    field_num++;
    
    data->longitude = strtold(fields[field_num], NULL);
    field_num++;

    data->easy_adress = malloc(strlen(fields[field_num]) + 1); // for '\0'
    strcpy(data->easy_adress, fields[field_num]);

    return data;
}

// Free wildlife data
void free_wildlife(wildlife_t *data){
    if (data == NULL) {
        return;
    }

    free(data->key);
    free(data->taxa);
    free(data->kingdom);
    free(data->phylum);
    free(data->class_name);
    free(data->order);
    free(data->family);
    free(data->genus);
    free(data->species);
    free(data->common_name);
    free(data->identification);
    free(data->data_resource);
    free(data->sighting_data);
    free(data->easy_adress);

    free(data);
}