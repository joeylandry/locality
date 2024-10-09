/**************************************************************
 *
 *                     uarray2b.c
 *
 *     Assignment: Locality
 *     Authors:    Joey Landry & Arshiya Lall 
 *     Date:       October 7, 2024
 *
 *     Contains implementation of the uarray2b.c class,
 *     a 2D blocked array using the Hanson UArray class as an 
 *     underlying data structure. 
 *
 ***************************************************************/

#include "uarray2b.h"
#include "uarray2.h"
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <uarray.h>

/* 
 * UArray2_T struct
 * Depicts a 2-dimensional unboxed array. 
 * Stores width and height of the array, the size of each element
 * and a Hanson `UArray_T` as a data structure
 */
struct UArray2b_T {
    int width; /* The width (num columns) of the array */
    int height; /* The height (num rows) of the array */
    int size; /* Size of data stored in each element of the array */
    int blocksize; /* Size of each block for blocked array */
    UArray2_T blocks; /* Hanson's UArray_T that holds data */
};

/********** UArray2b_new ********
 *
 * Allocates and returns a new blocked 2D array UArray2b_T containing 'width' 
 * by 'height' elements and each element is 'size' bytes and array is split
 * into blocks of size 'blocksize'
 *
 * Parameters:
 *      int width:     the number of columns in the array
 *      int height:    the number of rows in the array
 *      int size:      the byte size of each element
 *      int blocksize: number of elements per block's side (square dimension)
 *
 * Return: A new 'UArray2b_T' object illustrating the blocked 2D array
 *
 * Expects
 *      width, height, size, and blocksize must be greater than zero
 * 
 * Notes:
 *      - Checked runtime error if width, size, height, or blocksize are 
 *        invalid values
 *      - Hanson's 'UArray2_T' is used to create a grid of blocks; each block
 *        is created as a 'UArray_T'
 ************************/
UArray2b_T UArray2b_new(int width, int height, int size, int blocksize)
{
        /* ensure valid dimensions */
        assert(width > 0);
        assert(height > 0);
        assert(size > 0);
        assert(blocksize > 0);

        /* allocate memory for UArray2b_T struct (2D blocked array) */
        UArray2b_T uarray2b = (UArray2b_T)malloc(sizeof(struct UArray2b_T));
        assert(uarray2b != NULL);

        uarray2b->width = width;
        uarray2b->height = height;
        uarray2b->size = size;
        uarray2b->blocksize = blocksize; 

        /* calculate number of blocks required */
        int col_block = (width + blocksize - 1) / blocksize;
        int row_block = (height + blocksize - 1) / blocksize;

        uarray2b->blocks = UArray2_new(col_block, row_block, sizeof(UArray_T));

        for (int row = 0; row < row_block; row++) {
            for (int col = 0; col < col_block; col++) {
                UArray_T block = UArray_new(blocksize * blocksize, size);
                *(UArray_T *)UArray2_at(uarray2b->blocks, col, row) = block;
            }
        }

     return uarray2b;
}

/********** UArray2b_new_64K_block ********
 *
 * Allocates and returns 'UArray2b_T' (blocked 2D array) where the number
 * of elements is width x height, and each element is 'size' bytes. Array
 * is divided into blocks that optimize memory so that every block uses
 * around 64KB. 
 *
 * Parameters:
 *      int width:     number of columns in the array
 *      int size:      the number of rows in the array
 *      int blocksize: the byte size of each element
 *
 * Return: A new 'UArray2b_T' that represents the blocked 2D array
 *
 * Expects
 *      width, height, and a valid size (greater than 0)
 * 
 * Notes:
 *      - blocksize_max helps find the optimal block size needed so that 
 *        each block is within 64KB (65, 536 bytes)
 *      - blocksize_max is largest possible dimensions of a sqaure block 
 *        that allows block's total size to fit in the 64KB constraint
 *      - if 'blocksize_max' is < 1, set it to 1 to ensure a valid blocksize
 ************************/
UArray2b_T UArray2b_new_64K_block(int width, int height, int size) 
{
    assert(width > 0);
    assert(height > 0);
    assert(size > 0); 

    int blocksize_max = sqrt(64*1024 / size);
    if (blocksize_max < 1) {
        blocksize_max = 1; 
    }
    
    return UArray2b_new(width, height, size, blocksize_max);
}

/********** UArray2b_free ********
 *
 * All
 *
 * Parameters:
 *      UArray2b_T *array2b: pointer to block array that needs to be freed. 
 *                           pointer is set to 'NULL' once memory is free
 *
 * Return: None
 *
 * Expects
 *      'array2b' is a valid pointer to 'UArray2b_T' and is not 'NULL'
 * 
 * Notes: 
 *      - none
 *      
 ************************/
void UArray2b_free(UArray2b_T *array2b)
{
    assert(array2b != NULL && *array2b != NULL);

    int block_rows = UArray2_height((*array2b)->blocks);
    int block_cols = UArray2_width((*array2b)->blocks);

    /* free each block */
    for (int row = 0; row < block_rows; row++) {
        for (int col = 0; col < block_cols; col++) {
            UArray_T *block = UArray2_at((*array2b)->blocks, col, row);
            UArray_free(block);
        }
    }

    /* free the block array */
    UArray2_free(&(*array2b)->blocks);
    free(*array2b);
    *array2b = NULL;
}

/********** UArray2b_width ********
 *
 * Returns width, or number of columns, in blocked 2D array 'UArray2b_T'
 *
 * Parameters:
 *      UArray2b_T array2b: blocked 2D array whose width we must calculate   
 *
 * Return: width of 'array2b' (number of columns in the array)
 *
 * Expects
 *      'array2b' must be a valid, non-NULL argument
 * 
 * Notes:
 *      - checked runtime error if 'array2b' is NULL value
 *
 ************************/
int UArray2b_width(UArray2b_T array2b) 
{
    assert(array2b != NULL);
    return array2b->width; 
}

/********** UArray2b_height ********
 *
 * Returns height, or number of rows, in blocked 2D array 'UArray2b_T'
 *
 * Parameters:
 *      UArray2b_T array2b: blocked 2D array whose height we must calculate 
 *
 * Return: height of 'array2b' (number of rows in the array)
 *
 * Expects
 *      'array2b' must be a valid, non-NULL argument
 * 
 * Notes:
 *      - checked runtime error if 'array2b' is NULL value
 *      
 ************************/
int UArray2b_height(UArray2b_T array2b) 
{
    assert(array2b != NULL);
    return array2b->height;
}

/********** UArray2b_size ********
 *
 * Returns size (in bytes) of each element in 'UArray2b_T' 2D array
 *
 * Parameters:
 *      UArray2b_T array2b: blocked 2D array whose element size we must find
 *
 *
 * Return: size (in bytes) of every element in 'array2b'
 *
 * Expects
 *      'array2b' must be valid and non-NULL
 * 
 * Notes:
 *      - checked runtime error if 'array2b' is NULL value
 *      
 ************************/
int UArray2b_size(UArray2b_T array2b)
{
    assert(array2b != NULL);
    return array2b->size;
}

/********** UArray2b_blocksize ********
 *
 * Finds the block size of 'UArray2b_T.' Block size determines number of 
 * elements in a block. Each block is a square, meaning the dimensions are 
 * 'blocksize x blocksize'
 *
 * Parameters:
 *      UArray2b_T array2b: blocked, 2D array whose blocksize we must find
 *
 * Return: block size of 'array2b' or number of elements along width and
 *         height of a block
 *
 * Expects
 *      'array2b' must be a valid, non-NULL input
 * 
 * Notes:
 *      - checked runtime error if 'array2b' is NULL 
 *      
 ************************/
int UArray2b_blocksize(UArray2b_T array2b)
{
    assert(array2b != NULL);
    return array2b->blocksize;
}

/********** UArray2b_at ********
 *
 * Returns pointer to element at specific column and row in 2D array 
 * 'UArray2b_T'
 *
 * Parameters:
 *      UArray2b_T uarray2b: blocked 2D array where an element can be accessed
 *      int col:             column index of element to access (0 <= col < 
 *                           width) of uarray2b     
 *      int row:             row index of the element to access (0 <= row < 
 *                           height) of uarray2b     
 *
 * Return: void pointer to element at (col, row) position in the blocked array
 *
 * Expects
 *      - 'uarray2b' must be a valid, non-NULL input
 *      - col is within bounds of array width 
 *      - row is within bounds of array height
 * 
 * Notes:
 *      - checked runtime error if 'uarray2b' is NULL or if 'col' or 'row'
 *        is out of bounds
 *      
 ************************/
void *UArray2b_at(UArray2b_T uarray2b, int col, int row)
{
    assert(uarray2b != NULL);
    assert(col >= 0 && col < uarray2b->width);
    assert(row >= 0 && row < uarray2b->height);

    int block_col = col / uarray2b->blocksize;
    int block_row = row / uarray2b->blocksize;
    
    int i_block_col = col % uarray2b->blocksize;
    int i_block_row = row % uarray2b->blocksize;
    
    UArray_T *block = UArray2_at(uarray2b->blocks, block_col, block_row);
    int index = i_block_row * uarray2b->blocksize + i_block_col;

    return UArray_at(*block, index);
}

/********** UArray2b_map ********
 *
 * Iterates through each element in 'UArray2b_T' and applies the 'apply'
 * function to each element 
 *
 * Parameters:
 *      UArray2b_T uarray2b: blocked, 2D array whose elements will be inspected
 *      void apply(int col, int row, UArray2b_T uarray2b, void *elem, void 
 *      *cl): function provided by user to apply to every element in array 
 *      Parameters of 'apply':
 *              - 'col' and 'row': index to curr element's col and row position
 *              - 'uarray2b': blocked array that is traversed
 *              - 'elem': pointer to curr element in array 
 *              - 'cl': closure pointer to add any data needed by apply
 *      void *cl:           closure pointer inputted in apply function that 
 *                          stores any additional data 'apply' may need
 *
 * Return: none
 *
 * Expects
 *      'uarray2b' must be a valid, non-NULL input
 *      'apply' must be a non-NULL function pointer 
 * 
 * Notes:
 *      - none
 *      
 ************************/
void UArray2b_map(UArray2b_T uarray2b,
                  void apply(int col, int row, UArray2b_T uarray2b, 
                  void *elem, void *cl), void *cl)
{
    assert(uarray2b != NULL && apply != NULL);

    int row_block = UArray2_height(uarray2b->blocks);
    int col_block = UArray2_width(uarray2b->blocks);

    for (int row = 0; row < row_block; row++) {
        for (int col = 0; col < col_block; col++) {
            UArray_T *block = UArray2_at(uarray2b->blocks, col, row);
            int height = uarray2b->blocksize;
            int width =  uarray2b->blocksize;

            if ((row + 1) * uarray2b->blocksize > uarray2b->height) {
                height = uarray2b->height - row * uarray2b->blocksize;
            } 
            if ((col + 1) * uarray2b->blocksize > uarray2b->width) {
                width = uarray2b->width - col * uarray2b->blocksize;
            }

            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    int col2 = col * uarray2b->blocksize + j;
                    int row2 = row * uarray2b->blocksize + i;
                    int index = i * uarray2b->blocksize + j;
                    void *elem = UArray_at(*block, index);
                    apply(col2, row2, uarray2b, elem, cl);
                }
            }
        }
    }
}        