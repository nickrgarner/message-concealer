/**
 * @file conceal.c
 * @author Nick Garner, nrgarner
 * Main program flow for conceal functionality. Takes an input image in PPM format and
 * a text file with a message to conceal and hides the message in the given number of
 * low order bits of each image pixel. Prints resulting image to given output file.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "image.h"
#include "bits.h"

/** Initial size for a char array to hold the message to conceal */
#define INITMSGSIZE 10

/** Expected number of arguments for proper conceal usage */
#define NUMARGS 5

/** Indexes of the command line arguments */
#define MSGARG 1
#define INPUTARG 2
#define OUTPUTARG 3
#define BITSARG 4

/** Minimum and maximum acceptable bit amounts to replace */
#define MINBITS 1
#define MAXBITS 8

/**
 * Inserts bits of message chars into low order bits of image pixels
 * @param message Message to store in the image pixels
 * @param image Image to store the message in
 * @param numBits Number of low-order bits in pixels to replace with message bits
 * @param messageSpace Total amount of space allotted for message
 */
void insertMessage( unsigned char const *message, Image *image, int numBits, int
messageSpace )
{
  // Var to keep track of which pixel and color you're on
  int pixelColor = 0;

  // Var to keep track of which pixel bit you're on between message chars
  int pixelBit = 0;

  // Var to tally bits filled
  int bitsFilled = 0;

  // Current position in message
  int msgPos = 0;

  // Length of message
  int msgLen = strlen( ( char * ) message );

  // Get first char in message
  char currentChar = message[ msgPos++ ];

  // Add bits to pixels, loop until end of string
  for ( int i = 0; i < msgLen; i++ ) {
    for ( int k = 0; k < BITS_PER_BYTE; k++ ) {
      if ( pixelBit >= numBits ) {
        pixelColor++;
        pixelBit = 0;
      }
      image->color[ pixelColor ] = putBit( image->color[ pixelColor ], pixelBit++,
                                           getBit( currentChar, k ) );
      bitsFilled++;
    }
    currentChar = message[ msgPos++ ];
  }

  // Check for leftover message pixels and fill with zeroes
  if ( messageSpace > bitsFilled ) {
    while ( bitsFilled < messageSpace ) {
      if ( pixelBit >= numBits ) {
        pixelColor++;
        pixelBit = 0;
      }
      image->color[ pixelColor ] = putBit( image->color[ pixelColor ], pixelBit++, 0 );
      bitsFilled++;
    }
  }
}

/**
 * Reads text from a message file and returns it as a dynamically allocated char pointer.
 * @param filename File to read message from
 * @return Char pointer to message
 */
unsigned char *readMessage( char const *filename )
{
  FILE *fp = fopen( filename, "r" );

  // Dynamically allocate message pointer, resize as necessary
  unsigned char *message = ( unsigned char * ) malloc( INITMSGSIZE * sizeof( unsigned
          char ) );
  int capacity = INITMSGSIZE;
  int charCount = 0;

  unsigned char currentChar;

  while ( fscanf( fp, "%c", &currentChar ) != EOF ) {
    if ( charCount == capacity ) {
      capacity *= 2;
      message = ( unsigned char * ) realloc( message, capacity * sizeof( unsigned char
      ) );
    }
    message[ charCount++ ] = currentChar;
  }

  // Add null terminator
  if ( charCount == capacity ) {
    capacity *= 2;
    message = ( unsigned char * ) realloc( message, capacity * sizeof( unsigned char ) );
  }
  message[ charCount ] = '\0';

  // Close file
  fclose( fp );

  return message;
}

/**
 * Main program flow, checks for proper command line args and calls helper functions to
 * conceal message in target image and write to output file.
 * @param argc Number of command line arguments passed in
 * @param argv Content of command line arguments
 * @return 0 for successful program exit, 1 for exit with error
 */
int main( int argc, char *argv [] )
{
  // Check for proper usage
  if ( argc != NUMARGS ) {
    fprintf( stderr, "%s\n", "usage: conceal <input-message> <input-image> "
                             "<output-image> <bits>" );
    return EXIT_FAILURE;
  }

  // Check for proper bits value
  int bits = atoi( argv[ BITSARG ] );
  if ( bits < MINBITS || bits > MAXBITS ) {
    fprintf( stderr, "%s\n", "Invalid number of bits" );
    return EXIT_FAILURE;
  }


  // Read image, check message space
  Image *image = readImage( argv[ INPUTARG ] );
  unsigned char *message = readMessage( argv[ MSGARG ] );

  int messageSpace = image->rows * image->cols * PIXEL_WIDTH * bits;
  if ( strlen( ( char * ) message ) * BITS_PER_BYTE > messageSpace ) {
    fprintf( stderr, "%s\n", "Too few pixels to conceal message" );
    freeImage( image );
    free( message );
    return EXIT_FAILURE;
  }

  // Insert message bits into image
  insertMessage( message, image, bits, messageSpace );

  // Print image with concealed message to output file
  writeImage( image, argv[ OUTPUTARG ] );

  // Free allocs
  freeImage( image );
  free( message );
}
