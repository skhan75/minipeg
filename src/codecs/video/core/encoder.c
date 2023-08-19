#include "encoder.h"
#include <string.h>

#define QUANTIZATION_LEVELS 4
#define RLE_ENCODING 1

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

EncodedVideoData *encode_video(unsigned char *raw_buffer, int buffer_size, RawVideoConfig *config, int quantization_level) {
    // Initialize the quantization tables
    initialize_quantization_tables();

    // Number of bytes in a single frame
    int frame_size = config->width * config->height * config->bytes_per_pixel;

    // For simplicity, we're assuming the entire buffer is a perfect multiple of frame_size.
    // TODO - update later to handle cases where the buffer size is not a multiple of frame_size.
    // Maybe error handling or padding the buffer with zeros.
    int total_frames = buffer_size / frame_size;

    // This is just one approach where we handle frame-by-frame encoding.
    // Each frame is independently RLE encoded and quantized.
    // As a result, the EncodedVideoData->data will point to a concatenated array of RLE encoded frames.
    RLE* all_encoded_data = NULL; // holds all RLE sequences for all frames
    int total_encoded_size = 0;

    // Iterate Over Each Frame for Encoding
    for(int i = 0; i < total_frames; i++) {
        // gets a pointer to the start of the current frame we're processing. 
        // This frame will be encoded separately.
        size_t frame_offset = (size_t)i * frame_size;
        unsigned char *frame_buffer = raw_buffer + frame_offset;

        // Modify each pixel data in place, based on given quantization level
        apply_complex_quantization(frame_buffer, frame_size, quantization_level);

        // We adjust the size of the overall encoded data buffer to accommodate the newly encoded frame. 
        // Then, we copy the encoded frame data into the main buffer and update the size counter.
        int encoded_frame_size;
        RLE *frame_encoded_data = perform_rle_encoding(frame_buffer, frame_size, &encoded_frame_size);

        // It's a pointer that points to the memory space where the encoded frames are being stored. 
        // On the first iteration (when no frames have been encoded yet), it might be NULL 
        // (or not pointing to allocated memory).
        all_encoded_data = realloc(all_encoded_data, (total_encoded_size + frame_size) * sizeof(RLE));
        // Copying the New Encoded Data
        memcpy(all_encoded_data + total_encoded_size, frame_encoded_data, encoded_frame_size * sizeof(RLE));

        total_encoded_size += encoded_frame_size;
        free(frame_encoded_data);
    }

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
