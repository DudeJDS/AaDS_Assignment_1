#include "bit.h"
#include <assert.h>
#include <string.h>
/* Basic overview:
    Extracts the bit at some position w/o context of str len
*/
int getRawBit(char *arr, unsigned int bitIndex){
    unsigned int byte = bitIndex / BITS_PER_BYTE;
    unsigned int indexFromLeft = bitIndex % BITS_PER_BYTE;
    unsigned int offset = (BITS_PER_BYTE - (indexFromLeft) - 1) % BITS_PER_BYTE;
    unsigned char byteOfInterest = arr[byte];
    unsigned int offsetMask = (1 << offset);
    unsigned int maskedByte = (byteOfInterest & offsetMask);
    unsigned int bitOnly = maskedByte >> offset;
    return bitOnly;
}

int getBit(char *s, unsigned int bitIndex){
    assert(s);
    unsigned int byte = bitIndex / BITS_PER_BYTE;

    // Any bit past strings null terminator is junk
    // => return = 0 bcs. shorter len key differs from longer key once we have passed where it ends
    unsigned int len = strlen(s);
    if (byte > len) {
        return 0;
    }

    return getRawBit(s, bitIndex);
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