#ifndef PARSE_H
#define PARSE_H

/*----------- Dependency/s -----------*/
#include "data.h"


/*----------- Constant definitions -----------*/
#define IN_QUOTE 1
#define OUT_QUOTE 0
#define MAX_NUM_RECORDS 1353

/*----------- Data definitions -----------*/
typedef struct parsed_records parsed_records_t;
struct parsed_records {
    wildlife_t **records;
    int num_records;
};

/*----------- Function definitions -----------*/
parsed_records_t *parse_csv(FILE *inFile);

void free_parsed_records(parsed_records_t *parsed_records);
#endif