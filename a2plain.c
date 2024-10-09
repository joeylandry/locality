/**************************************************************
 *
 *                     uarray2_methods_plain.c
 *
 *     Assignment: Locality
 *     Authors:    Joey Landry & Arshiya Lall
 *     Date:       October 7, 2024
 *
 *     This file contains the implementation of the `A2Methods_T` interface 
 *     for unboxed 2D arrays using Hanson's `UArray2_T`
 *
 *
 **************************************************************/

#include <string.h>
#include <a2plain.h>
#include "uarray2.h"

/********** new ********
 *
 * Creates a new 2D array (`UArray2_T`) using the appropriate width, height, 
 * and element size in bytes.
 *
 * Parameters:
 *      int width:  number of columns in the array.
 *      int height: number of rows in the array.
 *      int size:   size (in bytes) of each element in the array
 *
 * Return:
 *      'A2Methods_UArray2` represents a new 2D array
 *
 * Expects:
 *      `width`, `height`, and `size` must be positive integers
 *
 * Notes:
 *      - 'UArray2_new` is used allocate memory for the array
 ************************/
static A2Methods_UArray2 new(int width, int height, int size)
{
        return UArray2_new(width, height, size);
}

/********** new_with_blocksize ********
 *
 * Creates a new 2D array with the specified width, height, and element size, 
 * and does not consider the `blocksize` parameter
 *
 * Parameters:
 *      int width:     number of columns in the array
 *      int height:    number of rows in the array
 *      int size:      size (in bytes) of each element in the array
 *      int blocksize: block size for arrays (ignored)
 *
 * Return:
 *      `A2Methods_UArray2` representing a new 2D array.
 *
 * Expects:
 *      `width`, `height`, and `size` must be positive integers
 *
 * Notes:
 *      - 'blocksize` parameter is not used
 *      - `UArray2_new` allocates memory for the array
 ************************/
static A2Methods_UArray2 new_with_blocksize(int width, int height, int size,
                                            int blocksize)
{
        (void) blocksize;
        return UArray2_new(width, height, size);
}

/********** a2free ********
 *
 * Frees all memory from 2D array `UArray2_T`.
 *
 * Parameters:
 *      A2Methods_UArray2 *uarray2: pointer to the array needed to be freed
 *
 * Return:
 *      none
 *
 * Expects:
 *      `uarray2` is a valid, non-NULL pointer to an `A2Methods_UArray2`.
 *
 * Notes:
 *      - UArray2_free` is called to free memory for array
 ************************/
static void a2free(A2Methods_UArray2 *uarray2)
{
        UArray2_free((UArray2_T *)uarray2);
}

/********** width ********
 *
 * Calculates number of columns in a 2D array `UArray2_T`
 *
 * Parameters:
 *      A2Methods_UArray2 uarray2: array whose width must be found
 *
 * Return:
 *      width (number of columns) in the array
 *
 * Expects:
 *      `uarray2` is a valid, non-NULL `A2Methods_UArray2`.
 ************************/
static int width(A2Methods_UArray2 uarray2)
{
        return UArray2_width(uarray2);
}

/********** height ********
 *
 * Calculates the number of rows in a 2D array `UArray2_T`.
 *
 * Parameters:
 *      A2Methods_UArray2 uarray2: array whose height must be found
 *
 * Return:
 *      The height (number of rows) of the array
 *
 * Expects:
 *      `uarray2` is a valid, non-NULL `A2Methods_UArray2`.
 ************************/
static int height(A2Methods_UArray2 uarray2)
{
        return UArray2_height(uarray2);
}

/********** size ********
 *
 * Calculates size (in bytes) of each element in `UArray2_T`.
 *
 * Parameters:
 *      A2Methods_UArray2 uarray2: array whose element size must be found
 *
 * Return:
 *      size (in bytes) of each element in the array
 *
 * Expects:
 *      `uarray2` is a valid, non-NULL `A2Methods_UArray2`.
 *
 ************************/
static int size(A2Methods_UArray2 uarray2)
{
        return UArray2_size(uarray2);
}

/********** blocksize ********
 *
 * Returns the block size of a 2D array
 *
 * Parameters:
 *      A2Methods_UArray2 uarray2: array whose block size must be found
 *
 * Return:
 *      block size of the array. If array is plain, return 1 
 *
 * Expects:
 *      `uarray2` is a valid, non-NULL `A2Methods_UArray2`.
 ************************/
static int blocksize(A2Methods_UArray2 uarray2)
{
        (void)uarray2;
        return 1;
}

/********** at ********
 *
 * Returns a pointer to the element at the specified column and row in array.
 *
 * Parameters:
 *      A2Methods_UArray2 uarray2: array needed to be accessed
 *      int col:                   column index of the element
 *      int row:                   row index of the element.
 *
 * Return:
 *      pointer to the element at the specific position in (col, row) 
 *      coordinates
 *
 * Expects:
 *      `uarray2` is a valid, non-NULL `A2Methods_UArray2`
 *      `col` and `row` are within the bounds of array
 ************************/
static A2Methods_Object *at(A2Methods_UArray2 uarray2, int col, int row)
{
        return UArray2_at(uarray2, col, row);
}

/********** map_row_major ********
 *
 * Applies a specified function to each element in a 2D array using 
 * row-major order
 *
 * Parameters:
 *      A2Methods_UArray2 uarray2: array that is traversed
 *      A2Methods_applyfun apply:  function to apply to every element in array
 *      void *cl:                  closure pointer for additional data
 *
 * Return:
 *      none
 *
 * Expects:
 *      `uarray2` is a valid, non-NULL `A2Methods_UArray2`
 *      `apply` is a valid function pointer
 ************************/
static void map_row_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
        UArray2_map_row_major(uarray2, (UArray2_applyfun*)apply, cl);
}

/********** map_col_major ********
 *
 * Applies a specific function to each element in a 2D array using
 * column-major order.
 *
 * Parameters:
 *      A2Methods_UArray2 uarray2: array that is traversed
 *      A2Methods_applyfun apply:  function to apply to every element in array
 *      void *cl:                  closure pointer for additional data
 *
 * Return:
 *      none
 *
 * Expects:
 *      `uarray2` is a valid, non-NULL `A2Methods_UArray2`.
 *      `apply` is a valid function pointer.
 ************************/
static void map_col_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
        UArray2_map_col_major(uarray2, (UArray2_applyfun*)apply, cl);
}

struct small_closure {
        A2Methods_smallapplyfun *apply; 
        void                    *cl;
};

/********** apply_small ********
 *
 * Adapts small function to apply to elements in a 2D array
 *
 * Parameters:
 *      int i:             column index of the element in array
 *      int j:             row index of the element in the array
 *      UArray2_T uarray2: array that needs to be traversed
 *      void *elem:        pointer to the current element in array
 *      void *vcl:         closure that has small function and additional data
 *
 * Return:
 *      none
 *
 * Expects:
 *      - 'elem' is a valid pointer to an element in 'uarray2'
 *      - 'vcl' cannot be a NULL pointer
 *      - 'cl->apply' is a valid function pointer
 *
 ************************/

static void apply_small(int i, int j, UArray2_T uarray2,
                        void *elem, void *vcl)
{
        struct small_closure *cl = vcl;
        (void)i;
        (void)j;
        (void)uarray2;
        cl->apply(elem, cl->cl);
}

/********** small_map_row_major ********
 *
 * Applies small function to every element in a 2D array using row-major order
 *
 * Parameters:
 *      A2Methods_UArray2 a2:          array that must be traversed
 *      A2Methods_smallapplyfun apply: small function to apply to every element
 *      void *cl:                      closure pointer to hold additional data
 *
 * Return:
 *      none
 *
 * Expects:
 *      `a2` is a valid, non-NULL `A2Methods_UArray2`.
 *
 ************************/
static void small_map_row_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
        struct small_closure mycl = { apply, cl };
        UArray2_map_row_major(a2, apply_small, &mycl);
}

/********** small_map_col_major ********
 *
 * Applies small function to every element in 2D array in column-major order
 *
 * Parameters:
 *      A2Methods_UArray2 a2:          array needed to be traversed
 *      A2Methods_smallapplyfun apply: small function to apply to every element
 *      void *cl:                      closure pointer for additional data
 *
 * Return:
 *      none
 *
 * Expects:
 *      `a2` is a valid, non-NULL `A2Methods_UArray2`.
 *
 ************************/
static void small_map_col_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
        struct small_closure mycl = { apply, cl };
        UArray2_map_col_major(a2, apply_small, &mycl);
}

/* Implementation of 'A2Methods_T' interface for unboxed 2D arrays */
static struct A2Methods_T uarray2_methods_plain_struct = {
        new,
        new_with_blocksize,
        a2free,
        width,
        height,
        size,
        blocksize,
        at,
        map_row_major,
        map_col_major,
        NULL,
        map_row_major,
        small_map_row_major,
        small_map_col_major,
        NULL,
        small_map_row_major
};

/* Exported pointer to the `A2Methods_T` struct, allowing access to the plain
darray methods */

A2Methods_T uarray2_methods_plain = &uarray2_methods_plain_struct;
