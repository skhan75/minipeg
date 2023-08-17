#include "encoder.h"
#include <string.h>

#define QUANTIZATION_LEVELS 4
static int quantization_tables[QUANTIZATION_LEVELS][256];
static int is_initialized = 0; // Global flag to check if tables are initialized

// Initialize the quantization tables
static void initialize_quantization_tables() {
    if (!is_initialized) {
        for (int level = 0; level < QUANTIZATION_LEVELS; level++) {
            for (int i = 0; i < 256; i++) {
                quantization_tables[level][i] = (i / (level + 1)) * (level + 1);
            }
        }
        is_initialized = 1;
    }
}

// Enhanced Quantization function
static void apply_complex_quantization(unsigned char *buffer, int buffer_size, int quantization_level) {
    if (quantization_level < 0 || quantization_level >= QUANTIZATION_LEVELS) {
        return; // invalid quantization level
    }

    for (int i = 0; i < buffer_size; i++) {
        buffer[i] = quantization_tables[quantization_level][buffer[i]];
    }
}

RLE *encode_video(const unsigned char *raw_buffer, int buffer_size, const RawVideoConfig *config, int *encoded_size, int quantization_level) {
    // Initialize the quantization tables
    initialize_quantization_tables();

    // Making a copy so that we don't modify the original raw_buffer.
    unsigned char *buffer_copy = (unsigned char *)malloc(buffer_size);
    if (!buffer_copy) {
        return NULL; // Error in memory allocation.
    }
    memcpy(buffer_copy, raw_buffer, buffer_size);

    // Step 1: Apply Quantization
    apply_complex_quantization(buffer_copy, buffer_size, quantization_level);

    // Step 2: Simple Run-Length Encoding
    RLE *encoded_buffer = (RLE *)malloc(sizeof(RLE) * buffer_size);
    if (!encoded_buffer) {
        free(buffer_copy);
        return NULL; // Error in memory allocation.
    }

    int j = 0; // Index for encoded buffer.
    for (int i = 0; i < buffer_size; i++) {
        int run_len = 1;
        while (i < buffer_size - 1 && buffer_copy[i] == buffer_copy[i + 1]) {
            run_len++;
            i++;
        }
        encoded_buffer[j].value = buffer_copy[i];
        encoded_buffer[j].count = run_len;
        j++;
    }

    free(buffer_copy);
    *encoded_size = j; // Update the actual size of the encoded buffer

    return encoded_buffer;
}
