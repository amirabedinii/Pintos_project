#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib/compression.h"

int main(void) {
    /* Test data with repeated patterns */
    const char* test_data = "AAAAABBBBBCCCCCDDDDDEEEEEFFFFF";
    size_t original_size = strlen(test_data);
    
    printf("Original data: %s\n", test_data);
    printf("Original size: %zu bytes\n", original_size);
    
    /* Compress the data */
    size_t compressed_size;
    void* compressed_data = compress_data(test_data, original_size, &compressed_size);
    
    if (!compressed_data) {
        printf("Compression failed!\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed_size / original_size * 100);
    
    /* Decompress the data */
    void* decompressed_data = decompress_data(compressed_data, compressed_size, original_size);
    
    if (!decompressed_data) {
        printf("Decompression failed!\n");
        free(compressed_data);
        return 1;
    }
    
    /* Verify the result */
    if (memcmp(test_data, decompressed_data, original_size) == 0) {
        printf("Compression test PASSED!\n");
    } else {
        printf("Compression test FAILED!\n");
    }
    
    /* Clean up */
    free(compressed_data);
    free(decompressed_data);
    
    return 0;
}