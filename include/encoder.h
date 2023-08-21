#ifndef ENCODER_H
#define ENCODER_H

#include <stdlib.h>

// Default BLOCK_SIZE
#define DEFAULT_BLOCK_SIZE 8

// Define video configuration structure
typedef struct {
    int width;
    int height;
    int bytes_per_pixel; // e.g., 3 for RGB, 4 for RGBA
    int frame_count;
} RawVideoConfig;

// Define RLE structure
typedef struct {
    unsigned char value;
    int count;
} RLE;

typedef struct {
    RLE *data;              // pointer to encoded data
    int data_size;          // Size of the encoded data in terms of the data type (#RLE structures)
    int encoding_type;      // Identifier for the encoding type (e.g., RLE for now, another algorithm, etc.)
    int quantization_level; // Quantization level used during encoding
} EncodedVideoData;

/**
 * Encode a raw video buffer using RLE and quantization.
 *
 * @param raw_buffer The input buffer with raw video data.
 * @param buffer_size The size of the input buffer.
 * @param config Configuration of the raw video data.
 * @param encoded_size Pointer to store the size of the encoded data.
 * @param quantization_level The level of quantization to apply (0 to 3).
 * @return Pointer to RLE encoded data. Caller is responsible for freeing this memory using free().
 */
EncodedVideoData *encode_video(unsigned char *raw_buffer, int buffer_size, RawVideoConfig *config, int quantization_level);

#endif // ENCODER_H
