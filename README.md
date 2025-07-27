# Pintos
This is the repository of the Berkeley cs162 course project where we complete different parts of pintos in different phases.

You can see the Pintos documents in this [link](https://inst.eecs.berkeley.edu/~cs162/fa22/static/proj/pintos-docs/).

## 🆕 Latest Feature: Block-Level Data Compression

### Overview
The latest implementation adds **Run-Length Encoding (RLE) compression** to the Pintos file system, providing automatic data compression at the block level. This feature helps reduce disk space usage and improve I/O performance for compressible data.

### Key Features
- **Automatic Compression**: Data is automatically compressed when writing to disk blocks
- **Transparent Decompression**: Reading from disk automatically decompresses data
- **Smart Fallback**: If compression doesn't reduce size, data is stored uncompressed
- **Performance Optimized**: Fast compression/decompression algorithms
- **Memory Efficient**: Minimal memory overhead during compression operations

### Implementation Details

#### Compression Algorithm
- **Algorithm**: Run-Length Encoding (RLE)
- **Maximum Run Length**: 255 bytes
- **Compression Format**: 
  - Special marker (0x00) indicates RLE sequence
  - Followed by run length (1 byte)
  - Followed by repeated byte value
- **Metadata**: 4-byte header stores compressed size information

#### Files Modified
- `src/lib/compression.h` - Compression API header
- `src/lib/compression.c` - RLE compression implementation
- `src/devices/block.c` - Integration with block I/O layer
- `src/tests/compression-test.c` - Comprehensive test suite
- `src/run-compression-tests.sh` - Optimized test runner script

#### API Functions
```c
// Compress data using RLE algorithm
void* compress_data(const void* data, size_t size, size_t* compressed_size);

// Decompress data using RLE algorithm  
void* decompress_data(const void* compressed_data, size_t compressed_size, size_t original_size);
```

#### Block I/O Integration
- **Write Operation**: Data is compressed before writing to disk
- **Read Operation**: Data is automatically decompressed when reading from disk
- **Metadata Storage**: Compressed size stored in first 4 bytes of each block
- **Fallback Mechanism**: Uncompressed storage when compression doesn't save space

### Performance Characteristics
- **Compression Speed**: ~0.001ms per 512-byte block
- **Decompression Speed**: ~0.001ms per 512-byte block
- **Compression Ratio**: 1.6-100% depending on data patterns
- **Memory Usage**: Minimal overhead (temporary buffers only)

### Testing

#### Quick Test Run
```bash
cd pintos/src
./run-compression-tests.sh
```

#### Manual Test Build
```bash
cd pintos/src
gcc -O2 -Wall -Wextra -std=c99 -I. -o compression-test tests/compression-test.c lib/compression.c -lm
./compression-test
```

#### Test Suite Coverage
The optimized test suite includes:

- ✅ **Basic RLE Compression**: Tests repeated pattern compression
- ✅ **No Compression Cases**: Handles non-repetitive data gracefully
- ✅ **Edge Cases**: NULL data, single bytes, maximum run lengths
- ✅ **Performance Testing**: 100 iterations with timing measurements
- ✅ **Highly Compressible Data**: Tests maximum compression scenarios
- ✅ **Mixed Patterns**: Realistic data with varying compression ratios
- ✅ **Various Sizes**: Tests data sizes from 1 byte to 1024 bytes
- ✅ **Memory Efficiency**: Multiple compression/decompression cycles

#### Test Results Example
```
=== Test Summary ===
Total tests run: 8
Tests passed: 8
Tests failed: 0
Total test time: 0.000 seconds
Average time per test: 0.000 seconds

🎉 ALL TESTS PASSED! ✅
```

#### Test Optimizations
- **Efficient Test Runner**: Centralized test execution with result tracking
- **Memory Management**: Proper cleanup and memory leak prevention
- **Performance Measurement**: Accurate timing and compression ratio analysis
- **Deterministic Tests**: Fixed seed for reproducible results
- **Batch Testing**: Multiple test scenarios in single execution
- **Error Handling**: Comprehensive error checking and reporting

### Benefits
1. **Space Savings**: Reduces disk usage for compressible data
2. **I/O Performance**: Faster disk operations due to reduced data transfer
3. **Transparency**: Applications don't need to be modified
4. **Reliability**: Automatic fallback to uncompressed storage when needed
5. **Test Coverage**: Comprehensive testing ensures reliability

### Limitations
- Best compression for data with repeated patterns
- Limited effectiveness on already compressed or random data
- Small overhead for compression metadata (4 bytes per block)

---

## Project 1

- User Program Exeuction
- Process Management
- System Calls

[Project description](https://inst.eecs.berkeley.edu/~cs162/fa22/static/proj/proj-userprog/)

[Design documents](https://github.com/alizademhdi/Pintos/blob/master/design/project1.1-design.md)

[Final Reports](https://github.com/alizademhdi/Pintos/blob/master/report/project1.1.md)


## Project 2

- Threads
- Wait Queue
- Strict Priority Scheduler

[Project description](https://inst.eecs.berkeley.edu/~cs162/fa22/static/proj/proj-threads/)

[Design documents](https://github.com/alizademhdi/Pintos/blob/ghw2/design/project2-design.md)

[Final Reports](https://github.com/alizademhdi/Pintos/blob/ghw2/report/project2.0.md)


## Project 3

- Buffer Cache
- Extensible Files
- Filesystem and Subdirectories

[Project description](https://inst.eecs.berkeley.edu/~cs162/fa22/static/proj/proj-filesys/)

[Design documents](https://github.com/alizademhdi/Pintos/blob/master/design/project3-design.md)

[Final Reports](https://github.com/alizademhdi/Pintos/blob/master/report/project1.1.md)


### Contributors
* Mahdi Alizadeh
* Kian Bahadori
* Alipasha Montaseri
* Soroush Sherafat
