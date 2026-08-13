#ifndef DATA_H
#define DATA_H

/*----------- Constant definitions -----------*/
#define MAX_FIELD_LEN 127
#define MAX_RECORD_LEN 511
#define MAX_NUM_FIELDS 16

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
    
    struct wildlife *next;
} wildlife_t;

/*----------- Function definitions -----------*/
wildlife_t *create_wildlife(char *fields[]);

#endif