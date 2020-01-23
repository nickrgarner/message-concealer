/**
 * @file bits.c
 * @author Nick Garner, nrgarner
 * Implementation file for bit manipulation functions. Supports reading a specified bit
 * from a number and changing the value of a specific bit.
 */

#include "bits.h"

/** Hex value to XOR with mask so that it can be used to clear a bit */
#define MASKXOR 0xFF

/**
 * Takes a given char and returns the bit value for the given bit index.
 * @param ch Char to inspect
 * @param n Index of bit value to return
 * @return Value at bit index of this char
 */
bool getBit( unsigned char ch, int n )
{
  int mask = 1;
  mask = mask << n;
  return ch & mask;
}

/** Changes bit number n to 0 or 1, returning the result.  If v == 1,
    then the bit is set; otherwise it's cleared.
    @param ch input byte to be modified.
    @param n bit number to change, 0 .. 7.
    @param v value bit number n should get, 0 or 1.
    @return value of ch, but with bit number n set to v.
*/
unsigned char putBit( unsigned char ch, int n, bool v )
{
  int mask = 1;
  mask = mask << n;
  if ( v ) {
    return ch | mask;
  } else {
    mask ^= MASKXOR;
    return ch & mask;
  }
}
