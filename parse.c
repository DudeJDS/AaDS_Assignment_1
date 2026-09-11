#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "parse.h"

/*----------- Input File Parse - Function -----------*/
/* Function goal:
        Primary: return an array of pointers to all data elements in the input file
        Secondary: keep track of total number of records*/
parsed_records_t *parse_csv(FILE *inFile){
    parsed_records_t *parsed_records = malloc(sizeof(parsed_records_t));
    assert(parsed_records);
    wildlife_t **arr_records = malloc(sizeof(wildlife_t*) * MAX_NUM_RECORDS);
    assert(arr_records);

    int n = 0;
    char line[MAX_RECORD_LEN];

    // Discard generic header
    fgets(line, sizeof(line), inFile); // Header just sits in this "buffer"
    // Process data line by line
    while (fgets(line, MAX_RECORD_LEN, inFile) != NULL) { // First call overwrites our "buffer" and we can handle meaningful input
        if (n == MAX_NUM_RECORDS) {
            fprintf(stderr, "Error: too many records\n");
            exit(1);
        }
        
        // Remove newline 
        line[strcspn(line, "\n")] = '\0';

        char *fields[MAX_NUM_FIELDS]; // Appropriate for entry to createwildlife 
        int field_cnt = 0;
        char *p = line;

        // Handle input with quotes
        while (field_cnt < MAX_NUM_FIELDS) {
            int quote_status = OUT_QUOTE;

            // Check to see if field starts with ""
            if (*p == '\"') {
                quote_status = IN_QUOTE;
                p++; // Skip quote
            }

            // Parse inputs  
            fields[field_cnt] = p;
            int len_field;
            if (quote_status == IN_QUOTE) {
                len_field = strcspn(p, "\"");
            } else {
                len_field = strcspn(p, ",");
            }
            p += len_field; // Moves p to the comma or quote mark

            // Check for change of field
            if (*p == ','){
                *p = '\0'; // Null terminate str in place so fields[field_cnt] has only the one field str before running into \0
                p++;
            } else if (*p == '\"') {
                *p = '\0';
                p++;
                if (*p == ',') {
                    p++; // Incase quote was on final field
                }
            }
            field_cnt++;
        }

        // Create new data for a node
        arr_records[n] = create_wildlife(fields);
        n++;
    }
    parsed_records->records = arr_records;
    parsed_records->num_records = n;
    return parsed_records;
}

void free_parsed_records(parsed_records_t *parsed_records) {
    if (parsed_records != NULL) {
        free(parsed_records->records);
        free(parsed_records);
    }

    return;
}