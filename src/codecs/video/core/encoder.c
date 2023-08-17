#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int width;
    int height;
    int bytes_per_pixel; // e.g., 3 for RGB, 4 for RGBA
    int frame_count;
} RawVideoConfig;

typedef struct {
    unsigned char value;
    int count;
} RLE;

RLE* encode_video(unsigned char *raw_buffer, int buffer_size, RawVideoConfig *config, int *encoded_size) {
    // Step 1: Quantization (using multiples of 4 for simplicity)
    for(int i=0; i<buffer_size; raw_buffer[i] = (raw_buffer[i] / 4) * 4;i++) {
        raw_buffer[i] = (raw_buffer[i] / 4) * 4;
    }
    
    // Step2: Simple Run Length Encoding
    RLE* encoded_buffer = (RLE*) malloc(sizeof(RLE) * buffer_size);
    int j = 0; // Index for encoded buffer.

    for(int i = 0; i < buffer_size; i++) {
        int run_len = 1;
        while (i < buffer_size - 1 && raw_buffer[i] == raw_buffer[i + 1]) {
            run_len++;
            i++;
        }
        encoded_buffer[j].value = raw_buffer[i];
        encoded_buffer[j].count = run_len;
        j++; // Move to the next slot in the encoded buffer
    }

    *encoded_size = j; // Update the actual size of the encoded buffer
    return encoded_buffer;
}