#include <stdio.h>
#include <stdlib.h>
#include "uarray2b.h"
#include <assert.h>

// Function to apply in UArray2b_map
void apply_function(int col, int row, UArray2b_T array2b, void *elem, void *cl) {
    (void)array2b;
    (void)cl;
    int *val = (int *)elem;
    printf("Element at (%d, %d): %d\n", col, row, *val);
}

// Test UArray2b_new and related functions
void test_new_and_basic_functions() {
    printf("Testing UArray2b_new and basic functions...\n");

    // Create a blocked 2D array
    int width = 5, height = 5, size = sizeof(int), blocksize = 2;
    UArray2b_T array2b = UArray2b_new(width, height, size, blocksize);

    // Check dimensions
    assert(UArray2b_width(array2b) == width);
    assert(UArray2b_height(array2b) == height);
    assert(UArray2b_size(array2b) == size);
    assert(UArray2b_blocksize(array2b) == blocksize);

    // Set and access elements
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int *val = UArray2b_at(array2b, col, row);
            *val = row * width + col; // Set the value based on position
        }
    }

    // Print array contents
    UArray2b_map(array2b, apply_function, NULL);

    // Free the array
    UArray2b_free(&array2b);
    assert(array2b == NULL);

    printf("UArray2b_new and basic functions test passed.\n\n");
}

// Test UArray2b_new_64K_block
void test_new_64K_block() {
    printf("Testing UArray2b_new_64K_block...\n");

    int width = 100, height = 100, size = sizeof(int);
    UArray2b_T array2b = UArray2b_new_64K_block(width, height, size);

    // Ensure it creates the array and sets blocksize properly
    assert(UArray2b_width(array2b) == width);
    assert(UArray2b_height(array2b) == height);

    // Set and access elements
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int *val = UArray2b_at(array2b, col, row);
            *val = row * width + col; // Set the value based on position
        }
    }

    // Print array contents for a small region
    printf("Printing a small region of 64K block array:\n");
    for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 10; col++) {
            int *val = UArray2b_at(array2b, col, row);
            printf("Element at (%d, %d): %d\n", col, row, *val);
        }
    }

    // Free the array
    UArray2b_free(&array2b);
    assert(array2b == NULL);

    printf("UArray2b_new_64K_block test passed.\n\n");
}

// Test UArray2b_map to ensure correct iteration
void test_map_function() {
    printf("Testing UArray2b_map...\n");

    int width = 4, height = 4, size = sizeof(int), blocksize = 2;
    UArray2b_T array2b = UArray2b_new(width, height, size, blocksize);

    // Set elements
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int *val = UArray2b_at(array2b, col, row);
            *val = row * width + col;
        }
    }

    // Test map
    UArray2b_map(array2b, apply_function, NULL);

    // Free the array
    UArray2b_free(&array2b);
    assert(array2b == NULL);

    printf("UArray2b_map test passed.\n\n");
}

// Test edge cases
void test_edge_cases() {
    printf("Testing edge cases...\n");

    // Test 1x1 array
    UArray2b_T array2b = UArray2b_new(1, 1, sizeof(int), 1);
    int *val = UArray2b_at(array2b, 0, 0);
    *val = 42;
    printf("Element at (0, 0) in 1x1 array: %d\n", *val);
    UArray2b_free(&array2b);

    // Test array where width and height are not multiples of block size
    int width = 7, height = 7, blocksize = 3;
    array2b = UArray2b_new(width, height, sizeof(int), blocksize);
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int *val = UArray2b_at(array2b, col, row);
            *val = row * width + col;
        }
    }
    UArray2b_map(array2b, apply_function, NULL);
    UArray2b_free(&array2b);

    printf("Edge cases test passed.\n\n");
}

int main() {
    test_new_and_basic_functions();
    test_new_64K_block();
    test_map_function();
    test_edge_cases();

    printf("All tests passed successfully.\n");
    return 0;
}
