#include "bit.h"
#include <assert.h>

int getBit(char *s, unsigned int bitIndex){
    assert(s);
    unsigned int byte = bitIndex / BITS_PER_BYTE;
    unsigned int indexFromLeft = bitIndex % BITS_PER_BYTE;
    /* 
        Since we split from the highest order bit first, the bit we are interested
        will be the highest order bit, rather than a bit that occurs at the end of the
        number. 
    */
    unsigned int offset = (BITS_PER_BYTE - (indexFromLeft) - 1) % BITS_PER_BYTE;
    unsigned char byteOfInterest = s[byte];
    unsigned int offsetMask = (1 << offset);
    unsigned int maskedByte = (byteOfInterest & offsetMask);
    /*
        The masked byte will still have the bit in its original position, to return
        either 0 or 1, we need to move the bit to the lowest order bit in the number.
    */
    unsigned int bitOnly = maskedByte >> offset;
    return bitOnly;
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