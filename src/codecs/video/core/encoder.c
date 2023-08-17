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

RLE *encode_video(unsigned char *raw_buffer, int buffer_size, RawVideoConfig *config, int *encoded_size) {
    // Step 1: Quantization (using multiples of 4 for simplicity)
    for(int i=0; i<buffer_size; i++) 
        raw_buffer[i] = (raw_buffer[i] / 4) * 4;

    
}