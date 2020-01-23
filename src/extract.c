/**
 * @file extract.c
 * @author Nick Garner, nrgarner
 * Main program flow for extract functionality. Takes an input image in PPM format with
 * low-order bits containing a concealed message and extracts the bits and prints them
 * to an output text file.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "image.h"
#include "bits.h"

/** Expected number of arguments for proper extract usage */
#define NUMARGS 4

/** Indexes of the command line arguments */
#define INPUTARG 1
#define OUTPUTARG 2
#define BITSARG 3

/** Minimum and maximum acceptable bit amounts to replace */
#define MINBITS 1
#define MAXBITS 8

/** Initial size of extracted message char array */
#define INITMSGSIZE 10

/**
 * Extracts the low order bits from the given image and constructs them into chars.
 * Chars are added to a heap-allocated message array and returned to the caller.
 * @param image Image to extract hidden message from.
 * @param numBits Number of low-order bits to read from each pixel RGB component.
 * @return Pointer to heap allocated extracted message.
 */
unsigned char *extractMessage ( Image *image, int numBits )
{
  // Dynamically allocate message pointer, resize as necessary
  unsigned char *message = ( unsigned char * ) malloc( INITMSGSIZE * sizeof( unsigned
          char ) );
  int capacity = INITMSGSIZE;
  int charCount = 0;

  unsigned char currentChar = ' ';

  // Var to keep track of which pixel you're on
  int pixelColor = 0;

  // Var to keep track of which pixel bit you're on between message chars
  int pixelBit = 0;

  // Var to keep track of how many pixel bits you've read
  int bitsRead = 0;

  // Total number of bits available to read
  int bitsMax = image->rows * image->cols * PIXEL_WIDTH * numBits;

  // Loop until hitting all zeroes or running out of bits to read
  while ( currentChar != 0 && bitsRead < bitsMax ) {
    for ( int i = 0; i < BITS_PER_BYTE; i++ ) {
      // If you've hit numBits, move to the next pixel color
      if ( pixelBit >= numBits ) {
        pixelColor++;
        pixelBit = 0;
      }
      currentChar = putBit( currentChar, i, getBit( *( image->color + pixelColor ),
              pixelBit++ ) );
    }
    // Check if array needs resize, add completed char to array
    if ( charCount == capacity ) {
      capacity *= 2;
      message = ( unsigned char * ) realloc( message, capacity * sizeof( unsigned char
      ) );
    }
    message[ charCount++ ] = currentChar;
  }

  // Done reading bits, add null terminator to message
  if ( charCount == capacity ) {
    capacity *= 2;
    message = ( unsigned char * ) realloc( message, capacity * sizeof( unsigned char
    ) );
  }
  message[ charCount ] = '\0';

  return message;
}

/**
 * Prints the given message to the output file specified from the command line.
 * @param message Pointer to the message to print to file.
 * @param filename Name of the file to print the message to.
 */
void msgToFile( unsigned char const *message, char const *filename )
{
  FILE *fp = fopen( filename, "w" );
  fprintf( fp, "%s", message );
  fclose( fp );
}

/**
 * Main extract program flow. Checks for proper command line args and calls helper
 * functions to extract hidden message in target image and write to output file.
 * @param argc Number of command line args passed in
 * @param argv Content of command line args
 * @return 0 for successful program exit, 1 for exit with error
 */
int main ( int argc, char *argv[] )
{
  // Check for proper usage
  if ( argc != NUMARGS ) {
    fprintf( stderr, "%s\n", "usage: extract <input-image> <output-message> <bits>" );
    return EXIT_FAILURE;
  }

  // Check for proper bits value
  int bits = atoi( argv[ BITSARG ] );
  if ( bits < MINBITS || bits > MAXBITS ) {
    fprintf( stderr, "%s\n", "Invalid number of bits" );
    return EXIT_FAILURE;
  }

  // Read image into struct
  Image *image = readImage( argv[ INPUTARG ] );

  // Extract message and print to output file
  unsigned char *message = extractMessage( image, atoi( argv[ BITSARG ] ) );
  msgToFile( message, argv[ OUTPUTARG ] );

  // Free allocs
  freeImage( image );
  free( message );
}
