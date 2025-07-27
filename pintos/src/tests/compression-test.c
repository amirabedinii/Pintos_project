#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "lib/compression.h"

/* Test configuration */
#define BLOCK_SECTOR_SIZE 512
#define MAX_TEST_SIZE 1024
#define PERFORMANCE_ITERATIONS 100

/* Test utilities */
#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("FAIL: %s\n", message); \
            return 1; \
        } \
    } while(0)

#define TEST_PASS(message) printf("PASS: %s\n", message)

/* Test result tracking */
static int total_tests = 0;
static int passed_tests = 0;

/* Efficient test runner */
static int run_test(const char* test_name, int (*test_func)(void)) {
    printf("\n--- %s ---\n", test_name);
    total_tests++;
    
    int result = test_func();
    if (result == 0) {
        passed_tests++;
        TEST_PASS(test_name);
    }
    return result;
}

/* Optimized compression/decompression test helper */
static int test_compression_roundtrip(const void* data, size_t size, const char* description) {
    size_t compressed_size;
    void* compressed_data = compress_data(data, size, &compressed_size);
    
    TEST_ASSERT(compressed_data != NULL, "Compression should succeed");
    
    void* decompressed_data = decompress_data(compressed_data, compressed_size, size);
    
    TEST_ASSERT(decompressed_data != NULL, "Decompression should succeed");
    
    /* Verify data integrity */
    int data_matches = (memcmp(data, decompressed_data, size) == 0);
    TEST_ASSERT(data_matches, "Data should match after compression/decompression");
    
    printf("  %s: %zu -> %zu bytes (%.1f%% ratio)\n", 
           description, size, compressed_size, 
           (float)compressed_size / size * 100);
    
    free(compressed_data);
    free(decompressed_data);
    return 0;
}

/* Test 1: Basic RLE compression with repeated patterns */
static int test_basic_rle_compression() {
    const char* test_data = "AAAAABBBBBCCCCCDDDDDEEEEEFFFFF";
    return test_compression_roundtrip(test_data, strlen(test_data), "Basic RLE");
}

/* Test 2: No compression case (no repeated patterns) */
static int test_no_compression() {
    const char* test_data = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    return test_compression_roundtrip(test_data, strlen(test_data), "No compression");
}

/* Test 3: Edge cases - optimized to test multiple cases efficiently */
static int test_edge_cases() {
    /* Test NULL data */
    size_t compressed_size;
    void* compressed_data = compress_data(NULL, 0, &compressed_size);
    TEST_ASSERT(compressed_data == NULL, "NULL data should return NULL");
    
    /* Test single byte */
    char single_byte = 'A';
    if (test_compression_roundtrip(&single_byte, 1, "Single byte") != 0) return 1;
    
    /* Test maximum run length efficiently */
    uint8_t* max_run = malloc(256);
    TEST_ASSERT(max_run != NULL, "Should allocate memory for max run test");
    
    memset(max_run, 'X', 255);
    max_run[255] = 'Y';
    
    int result = test_compression_roundtrip(max_run, 256, "Maximum run length");
    free(max_run);
    
    return result;
}

/* Test 4: Performance test - optimized with multiple iterations */
static int test_performance() {
    uint8_t* test_data = malloc(BLOCK_SECTOR_SIZE);
    TEST_ASSERT(test_data != NULL, "Should allocate memory for performance test");
    
    /* Generate test data once with a fixed pattern for reproducibility */
    for (int i = 0; i < BLOCK_SECTOR_SIZE; i++) {
        test_data[i] = (i % 4 == 0) ? 0xAA : (i % 256); /* Pattern with some compression potential */
    }
    
    printf("  Running %d iterations for performance measurement...\n", PERFORMANCE_ITERATIONS);
    
    clock_t start = clock();
    size_t total_compressed_size = 0;
    
    for (int i = 0; i < PERFORMANCE_ITERATIONS; i++) {
        size_t compressed_size;
        void* compressed_data = compress_data(test_data, BLOCK_SECTOR_SIZE, &compressed_size);
        
        TEST_ASSERT(compressed_data != NULL, "Performance test compression should succeed");
        total_compressed_size += compressed_size;
        
        void* decompressed_data = decompress_data(compressed_data, compressed_size, BLOCK_SECTOR_SIZE);
        
        TEST_ASSERT(decompressed_data != NULL, "Performance test decompression should succeed");
        TEST_ASSERT(memcmp(test_data, decompressed_data, BLOCK_SECTOR_SIZE) == 0, 
                    "Performance test data should match");
        
        free(compressed_data);
        free(decompressed_data);
    }
    
    clock_t total_time = clock() - start;
    double avg_time = (double)total_time / PERFORMANCE_ITERATIONS / CLOCKS_PER_SEC * 1000;
    double avg_compressed_size = (double)total_compressed_size / PERFORMANCE_ITERATIONS;
    
    printf("  Average time per block: %.3f ms\n", avg_time);
    printf("  Average compression ratio: %.1f%%\n", 
           avg_compressed_size / BLOCK_SECTOR_SIZE * 100);
    
    free(test_data);
    return 0;
}

/* Test 5: Highly compressible data - optimized pattern generation */
static int test_highly_compressible_data() {
    uint8_t* compressible_data = malloc(BLOCK_SECTOR_SIZE);
    TEST_ASSERT(compressible_data != NULL, "Should allocate memory for compressible data");
    
    /* Generate highly compressible pattern efficiently */
    for (int i = 0; i < BLOCK_SECTOR_SIZE; i += 4) {
        compressible_data[i] = 0x00;
        compressible_data[i+1] = 0x00;
        compressible_data[i+2] = 0x00;
        compressible_data[i+3] = 0x00;
    }
    
    int result = test_compression_roundtrip(compressible_data, BLOCK_SECTOR_SIZE, "Highly compressible");
    free(compressible_data);
    
    return result;
}

/* Test 6: Mixed data patterns - optimized with efficient pattern generation */
static int test_mixed_patterns() {
    uint8_t* mixed_data = malloc(BLOCK_SECTOR_SIZE);
    TEST_ASSERT(mixed_data != NULL, "Should allocate memory for mixed data");
    
    /* Generate mixed pattern efficiently */
    for (int i = 0; i < BLOCK_SECTOR_SIZE; i++) {
        if (i < 100) {
            mixed_data[i] = 0xFF; /* Repeated pattern */
        } else if (i < 200) {
            mixed_data[i] = i % 256; /* Varying pattern */
        } else if (i < 300) {
            mixed_data[i] = 0x00; /* Another repeated pattern */
        } else {
            mixed_data[i] = (i * 7) % 256; /* Pseudo-random pattern */
        }
    }
    
    int result = test_compression_roundtrip(mixed_data, BLOCK_SECTOR_SIZE, "Mixed patterns");
    free(mixed_data);
    
    return result;
}

/* Test 7: Stress test with various sizes - efficient batch testing */
static int test_various_sizes() {
    const int sizes[] = {1, 10, 50, 100, 256, 512, 1024};
    const int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    
    for (int i = 0; i < num_sizes; i++) {
        int size = sizes[i];
        uint8_t* data = malloc(size);
        TEST_ASSERT(data != NULL, "Should allocate memory for size test");
        
        /* Generate test data with some compression potential */
        for (int j = 0; j < size; j++) {
            data[j] = (j % 4 == 0) ? 0xAA : (j % 256); /* Pattern with some compression potential */
        }
        
        char description[64];
        snprintf(description, sizeof(description), "Size %d bytes", size);
        
        int result = test_compression_roundtrip(data, size, description);
        free(data);
        
        if (result != 0) return result;
    }
    
    return 0;
}

/* Test 8: Memory efficiency test */
static int test_memory_efficiency() {
    const int test_size = 1024;
    uint8_t* original_data = malloc(test_size);
    TEST_ASSERT(original_data != NULL, "Should allocate memory for efficiency test");
    
    /* Generate test data with some compression potential */
    for (int i = 0; i < test_size; i++) {
        original_data[i] = (i % 4 == 0) ? 0xAA : (i % 256);
    }
    
    /* Test multiple compression/decompression cycles */
    void* current_data = original_data;
    size_t current_size = test_size;
    
    for (int cycle = 0; cycle < 5; cycle++) {
        size_t compressed_size;
        void* compressed_data = compress_data(current_data, current_size, &compressed_size);
        
        TEST_ASSERT(compressed_data != NULL, "Efficiency test compression should succeed");
        
        void* decompressed_data = decompress_data(compressed_data, compressed_size, current_size);
        
        TEST_ASSERT(decompressed_data != NULL, "Efficiency test decompression should succeed");
        TEST_ASSERT(memcmp(current_data, decompressed_data, current_size) == 0, 
                    "Efficiency test data should match");
        
        /* Clean up and prepare for next cycle */
        if (current_data != original_data) {
            free(current_data);
        }
        free(compressed_data);
        free(decompressed_data);
        
        current_data = original_data;
        current_size = test_size;
    }
    
    free(original_data);
    printf("  Memory efficiency test: 5 compression cycles completed successfully\n");
    
    return 0;
}

int main(void) {
    printf("=== RLE Compression Test Suite (Optimized) ===\n");
    printf("Testing block-level data compression implementation\n");
    printf("Configuration: BLOCK_SECTOR_SIZE=%d, MAX_TEST_SIZE=%d\n\n", 
           BLOCK_SECTOR_SIZE, MAX_TEST_SIZE);
    
    clock_t start_time = clock();
    
    /* Run all tests efficiently */
    int failures = 0;
    failures += run_test("Basic RLE Compression", test_basic_rle_compression);
    failures += run_test("No Compression Case", test_no_compression);
    failures += run_test("Edge Cases", test_edge_cases);
    failures += run_test("Performance Test", test_performance);
    failures += run_test("Highly Compressible Data", test_highly_compressible_data);
    failures += run_test("Mixed Patterns", test_mixed_patterns);
    failures += run_test("Various Sizes", test_various_sizes);
    failures += run_test("Memory Efficiency", test_memory_efficiency);
    
    clock_t total_time = clock() - start_time;
    double test_duration = (double)total_time / CLOCKS_PER_SEC;
    
    printf("\n=== Test Summary ===\n");
    printf("Total tests run: %d\n", total_tests);
    printf("Tests passed: %d\n", passed_tests);
    printf("Tests failed: %d\n", total_tests - passed_tests);
    printf("Total test time: %.3f seconds\n", test_duration);
    printf("Average time per test: %.3f seconds\n", test_duration / total_tests);
    
    if (failures == 0) {
        printf("\n🎉 ALL TESTS PASSED! ✅\n");
        printf("RLE compression implementation is working correctly and efficiently.\n");
    } else {
        printf("\n❌ %d TEST(S) FAILED!\n", failures);
        printf("Please review the compression implementation.\n");
    }
    
    return failures;
}