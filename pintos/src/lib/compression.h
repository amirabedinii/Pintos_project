#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <stddef.h>
#include <stdint.h>

/* Compress data using LZ77 algorithm.
   DATA: Pointer to the data to compress
   SIZE: Size of the data in bytes
   COMPRESSED_SIZE: Pointer to store the size of compressed data
   Returns: Pointer to compressed data (must be freed by caller), or NULL on failure */
void* compress_data(const void* data, size_t size, size_t* compressed_size);

/* Decompress data using LZ77 algorithm.
   COMPRESSED_DATA: Pointer to the compressed data
   COMPRESSED_SIZE: Size of the compressed data in bytes
   ORIGINAL_SIZE: Size of the original uncompressed data
   Returns: Pointer to decompressed data (must be freed by caller), or NULL on failure */
void* decompress_data(const void* compressed_data, size_t compressed_size, size_t original_size);

#endif /* COMPRESSION_H */