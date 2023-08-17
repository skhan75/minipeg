#ifndef ENCODER_H
#define ENCODER_H

#include <stdlib.h>

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
RLE *encode_video(const unsigned char *raw_buffer, int buffer_size, const RawVideoConfig *config, int *encoded_size, int quantization_level);

#endif // ENCODER_H
