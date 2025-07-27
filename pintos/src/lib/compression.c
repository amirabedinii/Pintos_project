#include "compression.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Simple RLE (Run-Length Encoding) compression for better compatibility */
#define MAX_RUN_LENGTH 255

/* Compress data using simple RLE algorithm */
void* compress_data(const void* data, size_t size, size_t* compressed_size) {
    if (!data || size == 0 || !compressed_size) {
        return NULL;
    }
    
    const uint8_t* input = (const uint8_t*)data;
    uint8_t* output = malloc(size * 2); /* Worst case: no compression */
    if (!output) {
        return NULL;
    }
    
    int output_pos = 0;
    int input_pos = 0;
    
    while (input_pos < size) {
        uint8_t current_byte = input[input_pos];
        int run_length = 1;
        
        /* Count consecutive identical bytes */
        while (run_length < MAX_RUN_LENGTH && 
               input_pos + run_length < size && 
               input[input_pos + run_length] == current_byte) {
            run_length++;
        }
        
        if (run_length >= 3) {
            /* Write run-length encoded data */
            if (output_pos + 2 <= size * 2) {
                output[output_pos++] = 0; /* Special marker for RLE */
                output[output_pos++] = run_length;
                output[output_pos++] = current_byte;
            }
            input_pos += run_length;
        } else {
            /* Write literal data */
            if (output_pos + 1 <= size * 2) {
                output[output_pos++] = current_byte;
            }
            input_pos++;
        }
    }
    
    /* Resize output buffer to actual size */
    uint8_t* final_output = realloc(output, output_pos);
    if (!final_output) {
        free(output);
        return NULL;
    }
    
    *compressed_size = output_pos;
    return final_output;
}

/* Decompress data using simple RLE algorithm */
void* decompress_data(const void* compressed_data, size_t compressed_size, size_t original_size) {
    if (!compressed_data || compressed_size == 0 || original_size == 0) {
        return NULL;
    }
    
    const uint8_t* input = (const uint8_t*)compressed_data;
    uint8_t* output = malloc(original_size);
    if (!output) {
        return NULL;
    }
    
    int output_pos = 0;
    int input_pos = 0;
    
    while (input_pos < compressed_size && output_pos < original_size) {
        uint8_t current_byte = input[input_pos++];
        
        if (current_byte == 0 && input_pos + 1 < compressed_size) {
            /* RLE marker found */
            uint8_t run_length = input[input_pos++];
            uint8_t repeated_byte = input[input_pos++];
            
            /* Expand the run */
            for (int i = 0; i < run_length && output_pos < original_size; i++) {
                output[output_pos++] = repeated_byte;
            }
        } else {
            /* Literal byte */
            if (output_pos < original_size) {
                output[output_pos++] = current_byte;
            }
        }
    }
    
    return output;
}