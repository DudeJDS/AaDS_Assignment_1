#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "data.h"
#include "patricia.h"
#include "bit.h"
#include "parse.h"

// Define constants
#define IN_QUOTE 1
#define OUT_QUOTE 0

int main(int argc, char *argv[]) {

    // Check input command line is valid
    if (argc != 4) {
        return 1;
    }

    FILE *inFile = fopen(argv[2], "r");
    if (inFile == NULL) {
        fprintf(stderr, "Error: could not open %s\n", argv[2]);
        return 1;
    }
    FILE *outFile = fopen(argv[3], "w");
    if (outFile == NULL) {
        fprintf(stderr, "Error: could not open %s\n", argv[3]);
        return 1;
    }

    parsed_records_t *parsed_records = parse_csv(inFile);
    dict_t *dict = dict_build(parsed_records);    
    /*------------------------------ Parse & Search Query/s -------------------------------*/
    char query[MAX_FIELD_LEN + 1];
    while (fgets(query, sizeof(query), stdin) != NULL) {
        query[strcspn(query, "\r\n")] = '\0';

        int bit_cmps = 0;
        int str_cmps = 0;
        int node_cmps = 0;
        int records_found = 0;

        radix_search_by_key(dict->root, query, &bit_cmps, &str_cmps, &node_cmps, &records_found, outFile);
    }

    fclose(inFile);
    fclose(outFile);
    free_radix_tree(dict);
    free_parsed_records(parsed_records);
}
