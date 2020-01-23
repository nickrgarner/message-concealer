/**
 * @file image.h
 * @author Nick Garner, nrgarner
 * Header for the image component, with a representation for an Image and functions for
 * reading and writing images in PPM format.
*/

#ifndef _IMAGE_H_
#define _IMAGE_H_

/** PPM file format string */
#define FORMAT "P6"

/** Maximum color value */
#define MAX_COLOR 255

/** Number of intensity values per pixel. */
#define PIXEL_WIDTH 3

/** Representation for image file data */
typedef struct {
  /** number of rows */
  int rows;
  
  /** pixels per row */
  int cols;
  
  /** Dynamically allocated pixel data, rows * cols pixels, each with
      3 intensities (RGB), stored in row-major order. */
  unsigned char *color;
} Image;

#endif

/**
 * Reads in header info and pixel bits from an image PPM input file and stores them in
 * a heap-allocated image struct.
 * @param filename Name of the file to pull image info and bits from.
 * @return Image struct holding the pixel information along with dimensions.
 */
Image *readImage( char const *filename );

/**
 * Writes pixel bits from the given image struct to an image PPM output file.
 * @param image Struct containing the bits you want to output.
 * @param filename Name of the image PPM you want to write to.
 */
void writeImage( Image *image, char const *filename );

/**
 * Frees any heap memory associated with the given image struct, including the color
 * array and the struct itself.
 * @param image Image struct to free heap mem for.
 */
void freeImage( Image *image );
