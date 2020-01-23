/**
 * @file bits.h
 * @author Nick Garner, nrgarner
 * Header file for the bits.c component, with functions supporting copying a bit from
 * one character to another
*/

#ifndef _BITS_H_
#define _BITS_H_

#include <stdbool.h>

/** Number of bits per byte */
#define BITS_PER_BYTE 8


/** Return the value of bit number n from the given byte.
    @param ch byte from which to extract a bit.
    @param n bit number, 0 .. 7.
    @return value of the requested bit, 1 if it's set, 0 if not.
*/
bool getBit( unsigned char ch, int n );

/** Changes bit number n to 0 or 1, returning the result.  If v == 1,
    then the bit is set; otherwise it's cleared.
    @param ch input byte to be modified.
    @param n bit number to change, 0 .. 7.
    @param v value bit number n should get, 0 or 1.
    @return value of ch, but with bit number n set to v.
*/
unsigned char putBit( unsigned char ch, int n, bool v );

#endif
