#include <stdio.h>
#include <stdlib.h>
#include "a2plain.h"
#include "uarray2.h"

void print_element(int col, int row, A2Methods_UArray2 array, void *elem, void *cl) {
    (void)array;  // Cast to void to avoid unused variable warning
    (void)cl;
    int *value = elem;
    printf("Element at (%d, %d) = %d\n", col, row, *value);
}

void print_small_element(void *elem, void *cl) {
    (void)cl;
    int *value = elem;
    printf("Small map element = %d\n", *value);
}

int main(void) {
    // Step 1: Create an A2Methods_T object from the uarray2_methods_plain implementation
    A2Methods_T methods = uarray2_methods_plain;

    // Step 2: Create a new array (5x5) with size of each element being sizeof(int)
    int width = 5, height = 5, elem_size = sizeof(int);
    A2Methods_UArray2 array = methods->new(width, height, elem_size);
    
    if (!array) {
        fprintf(stderr, "Array creation failed!\n");
        return EXIT_FAILURE;
    }
    
    // Step 3: Test 'width' and 'height' methods
    printf("Array width: %d, height: %d\n", methods->width(array), methods->height(array));
    
    // Step 4: Test the 'at' method by setting some values in the array
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int *elem = methods->at(array, i, j);
            *elem = i + j;  // Set element value to the sum of the indices
        }
    }

    // Step 5: Test map_row_major
    printf("\nMapping row-major:\n");
    methods->map_row_major(array, print_element, NULL);

    // Step 6: Test map_col_major
    printf("\nMapping column-major:\n");
    methods->map_col_major(array, print_element, NULL);

    // Step 7: Test small_map_row_major
    printf("\nSmall map row-major:\n");
    methods->small_map_row_major(array, print_small_element, NULL);

    // Step 8: Test small_map_col_major
    printf("\nSmall map column-major:\n");
    methods->small_map_col_major(array, print_small_element, NULL);

    // Step 9: Test the size function (which returns the element size)
    printf("\nElement size: %d\n", methods->size(array));

    // Step 10: Test free method
    methods->free(&array);
    if (!array) {
        printf("\nArray successfully freed!\n");
    } else {
        printf("\nArray not freed!\n");
    }

    return EXIT_SUCCESS;
}
