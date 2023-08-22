#include "encoder.h"
#include "dct.h"
#include "color_conversion.h"
#include <string.h>

#define QUANTIZATION_LEVELS 4
#define RLE_ENCODING 1

static int quantization_tables[QUANTIZATION_LEVELS][256];
static int is_initialized = 0; // Global flag to check if tables are initialized

// Standard JPEG Quantization Table for Luminance
// Luminance table for grayscale or Y channel in YCbCr
static int LUMINANCE_QUANTIZATION_TABLE[8][8] = {
    {16, 11, 10, 16, 24, 40, 51, 61},
    {12, 12, 14, 19, 26, 58, 60, 55},
    {14, 13, 16, 24, 40, 57, 69, 56},
    {14, 17, 22, 29, 51, 87, 80, 62},
    {18, 22, 37, 56, 68, 109, 103, 77},
    {24, 35, 55, 64, 81, 104, 113, 92},
    {49, 64, 78, 87, 103, 121, 120, 101},
    {72, 92, 95, 98, 112, 100, 103, 99}};

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

    // Applying quantized values to the original buffer (in-place) of the video data.
    for (int i = 0; i < buffer_size; i++) {
        buffer[i] = quantization_tables[quantization_level][buffer[i]];
    }
}

// RLE Encoding function (returns a pointer to the encoded data)    
RLE* perform_rle_encoding(unsigned char* buffer, int buffer_size, int* encoded_size) {
    RLE *encoded_buffer = (RLE *)malloc(sizeof(RLE) * buffer_size);
    if (!encoded_buffer) {
        return NULL;
    }

    int j = 0;
    for (int i = 0; i < buffer_size; i++) {
        int run_len = 1;
        while (i < buffer_size - 1 && buffer[i] == buffer[i + 1]) {
            run_len++;
            i++;
        }
        encoded_buffer[j].value = buffer[i];
        encoded_buffer[j].count = run_len;
        j++;
    }
    *encoded_size = j; // communicate back the size of the encoded data
    return encoded_buffer;
}

EncodedVideoData *encode_video(
    unsigned char *raw_buffer, 
    int buffer_size, 
    RawVideoConfig *config, 
    int quantization_level,
    int block_size=0 // optional for dct block size
) {
    if(block_size == 0) block_size = DEFAULT_BLOCK_SIZE; // default block size

    // Initialize the quantization tables
    initialize_quantization_tables();

    // Initialize DCT
    DCTContext *dct_ctx = init_dct(block_size);
    if (!dct_ctx) return NULL;

    // Number of bytes in a single frame
    int frame_size = config->width * config->height * config->bytes_per_pixel;
    // For simplicity, we're assuming the entire buffer is a perfect multiple of frame_size.
    // TODO - update later to handle cases where the buffer size is not a multiple of frame_size.
    // Maybe error handling or padding the buffer with zeros.
    int total_frames = buffer_size / frame_size;

    // Assuming we're working with grayscale data for simplicity.
    // For RGB, you'd need to handle each channel separately. TODO - update later.
    unsigned char frame_block[block_size][block_size];
    double dct_output[block_size][block_size];

    // This is just one approach where we handle frame-by-frame encoding.
    // Each frame is independently RLE encoded and quantized.
    // As a result, the EncodedVideoData->data will point to a concatenated array of RLE encoded frames.
    RLE* all_encoded_data = NULL; // holds all RLE sequences for all frames
    int total_encoded_size = 0;

    // Iterate Over Each Frame for Encoding
    for (int frame = 0; frame < total_frames; frame++) {
        // gets a pointer to the start of the current frame we're processing. 
        size_t frame_offset = (size_t)frame * frame_size;
        unsigned char *frame_buffer = raw_buffer + frame_offset;

        // Conversion from RGB to YCBCr for the entire frame
        for(int pixel_idx = 0; pixel_idx < frame_size; pixel_idx += config->bytes_per_pixel) {
            unsigned char r = frame_buffer[pixel_idx];
            unsigned char g = frame_buffer[pixel_idx + 1];
            unsigned char b = frame_buffer[pixel_idx + 2];

            YCbCrPixel ycbcr_pixel;
            rgb_to_ycbcr(r, g, b, &ycbcr_pixel);

            // replace rgb with YcbCr color values
            frame_buffer[pixel_idx] = ycbcr_pixel.Y;
            frame_buffer[pixel_idx + 1] = ycbcr_pixel.Cb;
            frame_buffer[pixel_idx + 2] = ycbcr_pixel.Cr;
        }

        // Modify each pixel data in place, based on given quantization level
        apply_complex_quantization(frame_buffer, frame_size, quantization_level);

        for(int i=0; i<config->height; i+=block_size) {
            for(int j=0; j<config->width; j+=block_size) {
                // Extracting the block 
                for(int bi=0; bi<block_size; bi++) {
                    for(int bj=0; bj<block_size; bj++) {
                        frame_block[bi][bj] = frame_buffer[(i+bi)*config->width + (j+bj)];
                    }
                }
            }

            // Perform DCT on the block
            perform_dct(dct_ctx, frame_block, dct_output);
            // Quantize the DCT coefficients
            quantize_dct_coefficients(dct_output, LUMINANCE_QUANTIZATION_TABLE, block_size);

            // Flatten the 2D quantized DCT block to a 1D array
            // The flattening of a 2D block to a 1D array isn't strictly necessary for
            // the fundamental compression steps, but it is commonly done for Sequential Access
            // Simplicity and compatibility with algorithms like RLE and Huffman Encoding.
            unsigned char flattenedBlock[block_size * block_size];
            for(int bi = 0; bi < block_size; bi++) {
                for(int bj = 0; bj < block_size; bj++) {
                    flattenedBlock[bi * block_size + bj] = (unsigned char)dct_output[bi][bj];
                }
            }

            // We adjust the size of the overall encoded data buffer to accommodate the newly encoded frame.
            // Then, we copy the encoded frame data into the main buffer and update the size counter.
            int encoded_block_size;
            RLE *block_encoded_data = perform_rle_encoding(flattenedBlock, block_size * block_size, &encoded_block_size);

            // Resize the output buffer and copy the new RLE encoded block into it
            all_encoded_data = realloc(all_encoded_data, (total_encoded_size + encoded_block_size) * sizeof(RLE));
            memcpy(all_encoded_data + total_encoded_size, block_encoded_data, encoded_block_size * sizeof(RLE));

            total_encoded_size += encoded_block_size;
            free(block_encoded_data);
        }
    }

    // Cleanup
    free_dct(dct_ctx);

    EncodedVideoData* encoded_data = (EncodedVideoData*) malloc(sizeof(EncodedVideoData));
    if(!encoded_data) {
        free(all_encoded_data);
        return NULL;
    }
    encoded_data->data = all_encoded_data;
    encoded_data->data_size = total_encoded_size;
    encoded_data->encoding_type = RLE_ENCODING;
    encoded_data->quantization_level = quantization_level;

    return encoded_data;
}

// TODO - JPEG typically operates in the YCbCr color space where luminance (Y) and chrominance (Cb, Cr)
// are separated. If your video encoding is using a different color space, like RGB, the effect of the
// quantization table might not be optimal. In such a scenario, you might need to adjust the table or 
// switch to a color space like YCbCr.
// --> verify color space