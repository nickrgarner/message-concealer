/**
 * @file image.c
 * @author Nick Garner, nrgarner
 * Defines functionality for reading an input image file to a struct and writing an
 * image struct to an output file. Also defines freeImage function to restore heap
 * memory at end of program.
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "image.h"

#define FORMATLEN 2

#define HEADERARGS 4

/**
 * Reads in header info and pixel bits from an image PPM input file and stores them in
 * a heap-allocated image struct.
 * @param filename Name of the file to pull image info and bits from.
 * @return Image struct holding the pixel information along with dimensions.
 */
Image *readImage( char const *filename )
{
  // Try to open file
  FILE *fp = fopen( filename, "rb" );
  if ( fp == NULL) {
    // Error opening file
    perror( filename );
    exit( EXIT_FAILURE );
  }

  // Scan in image parameters
  char format[ FORMATLEN + 1 ];
  int width = 0;
  int height = 0;
  int maxIntensity = 0;
  int headerMatches = fscanf( fp, "%s\n%d %d\n%d\n", format, &width, &height,
          &maxIntensity );

  // Get number of chars after header
  int expectedChars = width * height * PIXEL_WIDTH;
  int headerChars = ftell( fp );
  fseek( fp, 0, SEEK_END );
  int charsAfterHeader = ftell( fp ) - headerChars;

  // Seek back to start of pixel chars
  fseek( fp, headerChars, SEEK_SET );

  // Check image properties for acceptable values
  if ( headerMatches != HEADERARGS || strcmp( format, FORMAT ) != 0 || width < 1 ||
  height < 1 || maxIntensity != MAX_COLOR || charsAfterHeader != expectedChars ) {
    // Image parameters are invalid
    fclose( fp );
    fprintf(stderr, "%s\n", "Invalid image file" );
    exit( EXIT_FAILURE );
  }

  // Create image object
  Image *image = ( Image * ) malloc( sizeof( Image ) );
  image->rows = height;
  image->cols = width;
  image->color = ( unsigned char * ) malloc( expectedChars * sizeof( unsigned char ) );
  fread( image->color, sizeof( unsigned char ), expectedChars, fp );

  // Close file
  fclose( fp );

  return image;
}

/**
 * Writes pixel bits from the given image struct to an image PPM output file.
 * @param image Struct containing the bits you want to output.
 * @param filename Name of the image PPM you want to write to.
 */
void writeImage( Image *image, char const *filename )
{
  // Open file, check for errors
  FILE *fp = fopen( filename, "wb" );
  if ( fp == NULL ) {
    // Error opening file
    perror( filename );
    exit( EXIT_FAILURE );
  }

  // Print header info
  fprintf( fp, "%s\n%d %d\n%d\n", FORMAT, image->cols, image->rows, MAX_COLOR );

  // Print pixel bits
  int numBits = image->cols * image->rows * PIXEL_WIDTH;
  fwrite( image->color, sizeof( unsigned char ), numBits, fp );

  // Close file
  fclose( fp );
}

/**
 * Frees any heap memory associated with the given image struct, including the color
 * array and the struct itself.
 * @param image Image struct to free heap mem for.
 */
void freeImage( Image *image )
{
  free( image->color );
  free( image );
}
