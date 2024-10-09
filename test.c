/**************************************************************
 *
 *                     ppmtrans.c
 *
 *     Assignment: Locality
 *     Authors:    Joey Landry & Arshiya Lall
 *     Date:       October 7, 2024
 *
 *     This file contains the implementation of 'ppmtrans', 
 *     which performs image transformations on PPM files, mainly  
 *     rotation using the A2Methods_T interface. 
 *
 *
 **************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "cputiming.h"

#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                       \
} while (false)

static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [-rotate <angle>] "
                        "[-{row,col,block}-major] "
		        "[-time time_file] "
		        "[filename]\n",
                        progname);
        exit(1);
}

void apply_copy(int col, int row, A2Methods_UArray2 uarray, void *elem, void *cl);
void apply_90(int col, int row, A2Methods_UArray2 uarray, void *elem, void *cl);
void apply_180(int col, int row, A2Methods_UArray2 uarray, void *elem, void *cl);
void apply_270(int col, int row, A2Methods_UArray2 uarray, void *elem, void *cl);

/********** apply_copy ********
 *
 * takes pixel data from original image and copies it into corresponding 
 * (col, row) position in new image 
 *
 * Parameters:
 *      int col:                  column index of pixel in original image
 *      int row:                  row index of pixel in original image
 *      A2Methods_UArray2 uarray: 2D array depicting original image.
 *      void *elem:               pointer to curr pixel (RGB info)
 *      void *cl:                 closure that points to Pnm_ppm image struct
 *
 * Return:
 *      none
 *
 * Expects:
 *      - `elem` points to valid pixel data.
 *      - `cl` points to a valid Pnm_ppm struct illustrating new image
 ************************/
void apply_copy(int col, int row, A2Methods_UArray2 uarray, void *elem, void *cl) {
        (void)uarray;
    Pnm_ppm new_image = (Pnm_ppm)cl; // Cast cl to Pnm_ppm
    A2Methods_T methods = (A2Methods_T)new_image->methods; // Explicit cast to remove const
    *(struct Pnm_rgb *)methods->at(new_image->pixels, col, row) = *(struct Pnm_rgb *)elem;
}

/********** apply_90 ********
 *
 * Helper function that rotates pixel data 90 degrees clockwise
 *
 * Parameters:
 *      int col:                  column index of the pixel in original image
 *      int row:                  row index of the pixel in original image
 *      A2Methods_UArray2 uarray: 2D array representing original image
 *      void *elem:               pointer to the current pixel (RGB) data
 *      void *cl:                 closure that points to Pnm_ppm image struct
 *
 * Return:
 *      none 
 *
 * Expects:
 *      - `elem` points to valid pixel data.
 *      - `cl` points to a valid Pnm_ppm struct representing the new image.
 ************************/
void apply_90(int col, int row, A2Methods_UArray2 uarray, void *elem, void *cl) {
    (void)uarray;
    Pnm_ppm new_image = (Pnm_ppm)cl;
    const struct A2Methods_T *methods = new_image->methods;

    int original_height = methods->height(uarray); // Original image height
    int new_col = original_height - row - 1;
    int new_row = col;

    // Calculating new indices for rotation
//     int new_col = methods->height(new_image->pixels) - row - 1;
//     int new_row = col;

    // Print the old and new positions for debugging
    fprintf(stderr, "Original: (%d, %d) -> New: (%d, %d)\n", col, row, new_col, new_row);

    // Access the element in the rotated position
    void *new_elem = methods->at(new_image->pixels, new_col, new_row);
    memcpy(new_elem, elem, sizeof(struct Pnm_rgb));
}



/********** apply_180 ********
 *
 * Helper function that rotates pixel data 180 degrees
 *
 * Parameters:
 *      int col:                  column index of the pixel in original image
 *      int row:                  row index of the pixel in original image
 *      A2Methods_UArray2 uarray: 2D array representing original image
 *      void *elem:               pointer to the current pixel (RGB) data
 *      void *cl:                 closure that points to Pnm_ppm image struct
 *
 * Return:
 *      none 
 *
 * Expects:
 *      - `elem` points to valid pixel data.
 *      - `cl` points to a valid Pnm_ppm struct representing the new image.
 ************************/
void apply_180(int col, int row, A2Methods_UArray2 uarray, void *elem, void *cl) {
        (void)uarray;
    Pnm_ppm new_image = (Pnm_ppm)cl;  // Cast cl to Pnm_ppm (which is a pointer to a struct)
    const struct A2Methods_T *methods = new_image->methods;  // Correctly access methods

    // Calculate new indices for 180-degree rotation
    int new_col = methods->width(new_image->pixels) - col - 1;
    int new_row = methods->height(new_image->pixels) - row - 1;

    // Access the element in the rotated position
    void *new_elem = methods->at(new_image->pixels, new_col, new_row);
    memcpy(new_elem, elem, sizeof(struct Pnm_rgb));
}

/********** apply_270 ********
 *
 * Helper function that rotates pixel data 270 degrees clockwise
 *
 * Parameters:
 *      int col:                  column index of the pixel in original image
 *      int row:                  row index of the pixel in original image
 *      A2Methods_UArray2 uarray: 2D array representing original image
 *      void *elem:               pointer to the current pixel (RGB) data
 *      void *cl:                 closure that points to Pnm_ppm image struct
 *
 * Return:
 *      none 
 *
 * Expects:
 *      - `elem` points to valid pixel data.
 *      - `cl` points to a valid Pnm_ppm struct representing the new image.
 ************************/
void apply_270(int col, int row, A2Methods_UArray2 uarray, void *elem, void *cl) {
        (void)uarray;
    Pnm_ppm new_image = (Pnm_ppm)cl;  // Cast cl to Pnm_ppm (which is a pointer to a struct)
    const struct A2Methods_T *methods = new_image->methods;  // Use const and access methods properly

    // Calculate new indices for 270-degree rotation
    int new_col = row;
    int new_row = methods->width(new_image->pixels) - col - 1;

    // Access the element in the rotated position
    struct Pnm_rgb *new_elem = methods->at(new_image->pixels, new_col, new_row);  // Correctly access destination pixel
    struct Pnm_rgb *old_elem = elem;  // Access source pixel

    // Copy the pixel data correctly
    new_elem->red = old_elem->red;
    new_elem->green = old_elem->green;
    new_elem->blue = old_elem->blue;
}

/********** main ********
 *
 * Executes the ppm image transformation based on user-specified options, such as 
 * rotation (0, 90, 180, 270 degrees), mapping methods, and timing.
 *
 * Parameters:
 *      int argc:              number of command-line arguments
 *      char *argv[]:          array of command-line arguments
 *
 * Return:
 *      returns 0 if successfully executed
 *
 * Expects:
 *      - command-line arguments follow appropriate format
 *      - acceptable rotation degree (0, 90, 180, 270) and mapping options 
 *        are stated
 * 
 * Notes:
 *      - writes the rotated image to standard output in binary PPM format
 *
 ************************/
int main(int argc, char *argv[])
{
        char *time_file_name = NULL;
        int   rotation       = 0;
        int   i;

        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map != NULL);

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-row-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_row_major, 
                                    "row-major");
                } else if (strcmp(argv[i], "-col-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_col_major, 
                                    "column-major");
                } else if (strcmp(argv[i], "-block-major") == 0) {
                        SET_METHODS(uarray2_methods_blocked, map_block_major,
                                    "block-major");
                } else if (strcmp(argv[i], "-rotate") == 0) {
                        if (!(i + 1 < argc)) {      /* no rotate value */
                                usage(argv[0]);
                        }
                        char *endptr;
                        rotation = strtol(argv[++i], &endptr, 10);
                        if (!(rotation == 0 || rotation == 90 ||
                            rotation == 180 || rotation == 270)) {
                                fprintf(stderr, 
                                        "Rotation must be 0, 90 180 or 270\n");
                                usage(argv[0]);
                        }
                        if (!(*endptr == '\0')) {    /* Not a number */
                                usage(argv[0]);
                        }
                } else if (strcmp(argv[i], "-time") == 0) {
                        if (!(i + 1 < argc)) {      /* no time file */
                                usage(argv[0]);
                        }
                        time_file_name = argv[++i];
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n", argv[0],
                                argv[i]);
                        usage(argv[0]);
                } else if (argc - i > 1) {
                        fprintf(stderr, "Too many arguments\n");
                        usage(argv[0]);
                } else {
                        break;
                }
        }

        /* Read the PPM image */
        FILE *file = (i < argc) ? fopen(argv[i], "r") : stdin;
        if (file == NULL) {
                fprintf(stderr, "Error: cannot open file %s\n", argv[i]);
                exit(EXIT_FAILURE);
        }


        Pnm_ppm image = Pnm_ppmread(file, methods);
        if (file != stdin) {
                fclose(file);
        }

        /* Array to hold rotated image */
        A2Methods_UArray2 rotated;
        if (rotation == 90 || rotation == 270) {
                rotated = methods->new(image->height, image->width, sizeof(struct Pnm_rgb));
        } else {
                rotated = methods->new(image->width, image->height, sizeof(struct Pnm_rgb));
        }

        /* transformed image struct */
        Pnm_ppm trans_image = malloc(sizeof(*trans_image));
        if (trans_image == NULL) {
                fprintf(stderr, "Error: Could not allocate memory for Pnm_ppm.\n");
                exit(EXIT_FAILURE);
        }

        /* initialize fields */
        trans_image->width = methods->width(rotated);
        trans_image->height = methods->height(rotated);
        trans_image->denominator = image->denominator;
        trans_image->pixels = rotated;
        trans_image->methods = methods;

        /* Debugging: Print dimensions of original and rotated images */
fprintf(stderr, "Original image: width = %d, height = %d\n", image->width, image->height);
fprintf(stderr, "Rotated image: width = %d, height = %d\n", trans_image->width, trans_image->height);
        

        /* Create timer */
        CPUTime_T timer = CPUTime_New();
        CPUTime_Start(timer);

        /* Complete the rotation */
        if (rotation == 0) {
                map(image->pixels, apply_copy, trans_image);
        } else if (rotation == 90) {
                map(image->pixels, apply_90, trans_image);
        } else if (rotation == 180) {
                map(image->pixels, apply_180, trans_image);
        } else if (rotation == 270) {
                map(image->pixels, apply_270, trans_image);
        }

        /* Stop timer */ 
        double total_time = CPUTime_Stop(timer);
        CPUTime_Free(&timer);

        /* Calculate total number of pixels */
        int total_pixels = image->width * image->height;
        double time_per_pixel = total_time / total_pixels;
        

       /* If timing file is specified, write timing information to it */
        if (time_file_name != NULL) {
                FILE *file_time = fopen(time_file_name, "a");
                if (file_time == NULL) {
                        perror("Unable to open timing file");
                        exit(1);
                }

                /* Append rotation and timing information */
                fprintf(file_time, "Rotation: %d degrees\n", rotation);
                fprintf(file_time, "Width: %d, Height: %d\n", image->width, image->height);
                fprintf(file_time, "Total time: %.0f nanoseconds\n", total_time);
                fprintf(file_time, "Total pixels: %d\n", total_pixels);
                fprintf(file_time, "Time per pixel: %.3f nanoseconds\n\n", time_per_pixel);
                fclose(file_time);
        } 

        Pnm_ppmwrite(stdout, trans_image);

        /* Free memory */
        free(trans_image);
        Pnm_ppmfree(&image);
        methods->free(&rotated);

        return 0; 
}