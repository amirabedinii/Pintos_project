#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "lib/compression.h"

/* Test utilities */
#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("FAIL: %s\n", message); \
            return 1; \
        } else { \
            printf("PASS: %s\n", message); \
        } \
    } while(0)

#define BLOCK_SECTOR_SIZE 512

/* Test 1: Basic RLE compression with repeated patterns */
int test_basic_rle_compression() {
    printf("\n=== Test 1: Basic RLE Compression ===\n");
    
    const char* test_data = "AAAAABBBBBCCCCCDDDDDEEEEEFFFFF";
    size_t original_size = strlen(test_data);
    
    printf("Original data: %s\n", test_data);
    printf("Original size: %zu bytes\n", original_size);
    
    size_t compressed_size;
    void* compressed_data = compress_data(test_data, original_size, &compressed_size);
    
    TEST_ASSERT(compressed_data != NULL, "Compression should succeed");
    TEST_ASSERT(compressed_size < original_size, "Compressed size should be smaller than original");
    
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (float)compressed_size / original_size * 100);
    
    void* decompressed_data = decompress_data(compressed_data, compressed_size, original_size);
    
    TEST_ASSERT(decompressed_data != NULL, "Decompression should succeed");
    TEST_ASSERT(memcmp(test_data, decompressed_data, original_size) == 0, 
                "Decompressed data should match original");
    
    free(compressed_data);
    free(decompressed_data);
    return 0;
}

/* Test 2: No compression case (no repeated patterns) */
int test_no_compression() {
    printf("\n=== Test 2: No Compression Case ===\n");
    
    const char* test_data = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    size_t original_size = strlen(test_data);
    
    printf("Original data: %s\n", test_data);
    printf("Original size: %zu bytes\n", original_size);
    
    size_t compressed_size;
    void* compressed_data = compress_data(test_data, original_size, &compressed_size);
    
    TEST_ASSERT(compressed_data != NULL, "Compression should succeed even with no patterns");
    
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (float)compressed_size / original_size * 100);
    
    void* decompressed_data = decompress_data(compressed_data, compressed_size, original_size);
    
    TEST_ASSERT(decompressed_data != NULL, "Decompression should succeed");
    TEST_ASSERT(memcmp(test_data, decompressed_data, original_size) == 0, 
                "Decompressed data should match original");
    
    free(compressed_data);
    free(decompressed_data);
    return 0;
}

/* Test 3: Edge cases */
int test_edge_cases() {
    printf("\n=== Test 3: Edge Cases ===\n");
    
    /* Test empty data */
    size_t compressed_size;
    void* compressed_data = compress_data(NULL, 0, &compressed_size);
    TEST_ASSERT(compressed_data == NULL, "NULL data should return NULL");
    
    /* Test single byte */
    char single_byte = 'A';
    compressed_data = compress_data(&single_byte, 1, &compressed_size);
    TEST_ASSERT(compressed_data != NULL, "Single byte compression should succeed");
    
    void* decompressed_data = decompress_data(compressed_data, compressed_size, 1);
    TEST_ASSERT(decompressed_data != NULL, "Single byte decompression should succeed");
    TEST_ASSERT(memcmp(&single_byte, decompressed_data, 1) == 0, 
                "Single byte should match after compression/decompression");
    
    free(compressed_data);
    free(decompressed_data);
    
    /* Test maximum run length */
    char max_run[256];
    memset(max_run, 'X', 255);
    max_run[255] = 'Y';
    
    compressed_data = compress_data(max_run, 256, &compressed_size);
    TEST_ASSERT(compressed_data != NULL, "Maximum run length compression should succeed");
    
    decompressed_data = decompress_data(compressed_data, compressed_size, 256);
    TEST_ASSERT(decompressed_data != NULL, "Maximum run length decompression should succeed");
    TEST_ASSERT(memcmp(max_run, decompressed_data, 256) == 0, 
                "Maximum run length should match after compression/decompression");
    
    free(compressed_data);
    free(decompressed_data);
    
    return 0;
}

/* Test 4: Random data performance test */
int test_random_data_performance() {
    printf("\n=== Test 4: Random Data Performance Test ===\n");
    
    /* Generate random data */
    uint8_t* random_data = malloc(BLOCK_SECTOR_SIZE);
    TEST_ASSERT(random_data != NULL, "Should allocate memory for random data");
    
    srand(time(NULL));
    for (int i = 0; i < BLOCK_SECTOR_SIZE; i++) {
        random_data[i] = rand() % 256;
    }
    
    printf("Generated %d bytes of random data\n", BLOCK_SECTOR_SIZE);
    
    clock_t start = clock();
    size_t compressed_size;
    void* compressed_data = compress_data(random_data, BLOCK_SECTOR_SIZE, &compressed_size);
    clock_t compress_time = clock() - start;
    
    TEST_ASSERT(compressed_data != NULL, "Random data compression should succeed");
    
    start = clock();
    void* decompressed_data = decompress_data(compressed_data, compressed_size, BLOCK_SECTOR_SIZE);
    clock_t decompress_time = clock() - start;
    
    TEST_ASSERT(decompressed_data != NULL, "Random data decompression should succeed");
    TEST_ASSERT(memcmp(random_data, decompressed_data, BLOCK_SECTOR_SIZE) == 0, 
                "Random data should match after compression/decompression");
    
    printf("Compression time: %.3f ms\n", (double)compress_time / CLOCKS_PER_SEC * 1000);
    printf("Decompression time: %.3f ms\n", (double)decompress_time / CLOCKS_PER_SEC * 1000);
    printf("Compression ratio: %.2f%%\n", (float)compressed_size / BLOCK_SECTOR_SIZE * 100);
    
    free(random_data);
    free(compressed_data);
    free(decompressed_data);
    
    return 0;
}

/* Test 5: Highly compressible data */
int test_highly_compressible_data() {
    printf("\n=== Test 5: Highly Compressible Data ===\n");
    
    /* Create highly compressible data with many repeated patterns */
    uint8_t* compressible_data = malloc(BLOCK_SECTOR_SIZE);
    TEST_ASSERT(compressible_data != NULL, "Should allocate memory for compressible data");
    
    /* Fill with repeated patterns */
    for (int i = 0; i < BLOCK_SECTOR_SIZE; i += 4) {
        compressible_data[i] = 0x00;
        compressible_data[i+1] = 0x00;
        compressible_data[i+2] = 0x00;
        compressible_data[i+3] = 0x00;
    }
    
    printf("Generated %d bytes of highly compressible data\n", BLOCK_SECTOR_SIZE);
    
    size_t compressed_size;
    void* compressed_data = compress_data(compressible_data, BLOCK_SECTOR_SIZE, &compressed_size);
    
    TEST_ASSERT(compressed_data != NULL, "Highly compressible data compression should succeed");
    TEST_ASSERT(compressed_size < BLOCK_SECTOR_SIZE, "Compressed size should be significantly smaller");
    
    void* decompressed_data = decompress_data(compressed_data, compressed_size, BLOCK_SECTOR_SIZE);
    
    TEST_ASSERT(decompressed_data != NULL, "Highly compressible data decompression should succeed");
    TEST_ASSERT(memcmp(compressible_data, decompressed_data, BLOCK_SECTOR_SIZE) == 0, 
                "Highly compressible data should match after compression/decompression");
    
    printf("Compression ratio: %.2f%%\n", (float)compressed_size / BLOCK_SECTOR_SIZE * 100);
    printf("Space saved: %.2f%%\n", (float)(BLOCK_SECTOR_SIZE - compressed_size) / BLOCK_SECTOR_SIZE * 100);
    
    free(compressible_data);
    free(compressed_data);
    free(decompressed_data);
    
    return 0;
}

/* Test 6: Integration test with block sector simulation */
int test_block_sector_integration() {
    printf("\n=== Test 6: Block Sector Integration Test ===\n");
    
    /* Simulate a block sector with mixed data */
    uint8_t* block_data = malloc(BLOCK_SECTOR_SIZE);
    TEST_ASSERT(block_data != NULL, "Should allocate memory for block data");
    
    /* Fill with a realistic pattern: some repeated, some random */
    for (int i = 0; i < BLOCK_SECTOR_SIZE; i++) {
        if (i < 100) {
            block_data[i] = 0xFF; /* Repeated pattern */
        } else if (i < 200) {
            block_data[i] = i % 256; /* Varying pattern */
        } else if (i < 300) {
            block_data[i] = 0x00; /* Another repeated pattern */
        } else {
            block_data[i] = rand() % 256; /* Random data */
        }
    }
    
    printf("Generated %d bytes of mixed block data\n", BLOCK_SECTOR_SIZE);
    
    size_t compressed_size;
    void* compressed_data = compress_data(block_data, BLOCK_SECTOR_SIZE, &compressed_size);
    
    TEST_ASSERT(compressed_data != NULL, "Block data compression should succeed");
    
    void* decompressed_data = decompress_data(compressed_data, compressed_size, BLOCK_SECTOR_SIZE);
    
    TEST_ASSERT(decompressed_data != NULL, "Block data decompression should succeed");
    TEST_ASSERT(memcmp(block_data, decompressed_data, BLOCK_SECTOR_SIZE) == 0, 
                "Block data should match after compression/decompression");
    
    printf("Compression ratio: %.2f%%\n", (float)compressed_size / BLOCK_SECTOR_SIZE * 100);
    
    free(block_data);
    free(compressed_data);
    free(decompressed_data);
    
    return 0;
}

int main(void) {
    printf("=== RLE Compression Test Suite ===\n");
    printf("Testing block-level data compression implementation\n\n");
    
    int failures = 0;
    
    failures += test_basic_rle_compression();
    failures += test_no_compression();
    failures += test_edge_cases();
    failures += test_random_data_performance();
    failures += test_highly_compressible_data();
    failures += test_block_sector_integration();
    
    printf("\n=== Test Summary ===\n");
    if (failures == 0) {
        printf("ALL TESTS PASSED! ✅\n");
        printf("RLE compression implementation is working correctly.\n");
    } else {
        printf("%d TEST(S) FAILED! ❌\n", failures);
        printf("Please review the compression implementation.\n");
    }
    
    return failures;
}