#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "data.h"
#include "list.h"
#include "bit.h"


// Define constants
#define IN_QUOTE 1
#define OUT_QUOTE 0
#define NOT_FOUND 'NOTFOUND'

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

    char line[MAX_RECORD_LEN];

    // Discard generic header
    fgets(line, sizeof(line), inFile); // Header just sits in this "buffer"
    node_t *head = NULL;
    node_t *tail = NULL;
    int size_list = 0;
    // Process data line by line
    while (fgets(line, MAX_RECORD_LEN, inFile) != NULL) { // First call overwrites our "buffer" and we can handle meaningful input
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
        wildlife_t *new_data = create_wildlife(fields);
        if (head == NULL) {
            head = create_node(new_data); //If first node in list make create node and let it be the head
            tail = head; //As per linked list structure
            size_list++;
        } else {
            tail->next = create_node(new_data); //If not first node in the list make the prev node now point here
            tail = tail->next;
            size_list++;
        } 
    }

    list_t *list = create_list(head, tail, size_list);
    // Insert into dictionary
    // Read queries
    // Print output
}